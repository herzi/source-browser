/* This file is part of source browser
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@gnome-de.org>
 *
 * Copyright (C) 2007  Sven Herzberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "sb-main.h"

#include <gtk/gtk.h>

#include "sb-callback-data.h"
#include "sb-display.h"
#include "sb-progress.h"
#include "sb-window.h"

#include <glib/gi18n.h>

static guint io = 0;

static gboolean io_watch_cb (GIOChannel  * channel,
			     GIOCondition  condition,
			     gpointer      data);

static void
watch_cb (GPid pid,
	  gint status_,
	  gpointer data)
{
	GIOChannel* channel = sb_callback_data_peek (data, "channel");
	GtkWidget * window  = sb_callback_data_peek (data, "window");
	g_print ("pre-done.\n");
	if (io) {
		g_source_remove (io);
		while (io_watch_cb (channel, G_IO_IN, sb_window_get_display (window)))
			; // parse trailing lines
	}
	g_print ("done.\n");
	g_spawn_close_pid (pid);
}

static gboolean
io_watch_cb (GIOChannel  * channel,
	     GIOCondition  condition,
	     gpointer      data)
{
	GIOStatus state = G_IO_STATUS_NORMAL;
	static GString* string = NULL;
	static gchar* revision = NULL;
	gunichar read = 0;

	if (G_UNLIKELY (!string)) {
		string = g_string_new ("");
	}

	state = g_io_channel_read_unichar (channel, &read, NULL);
	if (G_LIKELY (read != '\n')) {
		g_string_append_unichar (string, read);
	} else {
		if (!revision) {
			// "<40-byte hex sha1> <sourceline> <resultline> <num_lines>"
			gchar** words = g_strsplit (string->str, " ", -1);
			SbDisplay* self = SB_DISPLAY (data);
			revision = g_strdup (words[0]);
			g_signal_emit_by_name (self,
					       "load-progress",
					       atoi (words[3]));
			g_strfreev (words);
		} else if (g_str_has_prefix (string->str, "filename ")) {
			g_print ("%s\n", revision);
			g_free (revision);
			revision = NULL;
#if 0
		} else {
			// FIXME: meta-information about the commit
			g_debug ("Got unknown line: %s",
				 string->str);
#endif
		}
		g_string_free (string, TRUE);
		string = NULL;
	}

	if (state == G_IO_STATUS_NORMAL) {
		return TRUE;
	} else {
		io = 0;
		return FALSE;
	}
}

void
load_history (GtkWidget  * window,
	      gchar const* file_path)
{
	gchar* working_folder = g_path_get_dirname (file_path);
	gchar* argv[] = {
		"git-blame",
		"--incremental",
		"-M",
		"-C",
		NULL,
		NULL
	};
	GPid pid = 0;
	gint out_fd = 0;
	gpointer* channel_and_window;

	// FIXME: this is a bug in GtkTextView (it doesn't swallow the trailing \n)
	sb_progress_set_status (SB_PROGRESS (sb_window_get_status (window)), 1);

	argv[2] = g_path_get_basename (file_path);
	gdk_spawn_on_screen_with_pipes (gtk_widget_get_screen (window),
			     working_folder,
			     argv,
			     NULL,
			     G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
			     NULL,
			     NULL,
			     &pid,
			     NULL,
			     &out_fd,
			     NULL,
			     NULL); // FIXME: error, pipes
	GIOChannel* out_chan = g_io_channel_unix_new (out_fd);
	g_io_channel_set_close_on_unref (out_chan, TRUE);
	g_free (argv[2]);
	g_free (working_folder);

	io = g_io_add_watch (out_chan,
			     G_IO_IN | G_IO_PRI,
			     io_watch_cb,
			     sb_window_get_display (window));

	channel_and_window = sb_callback_data_new ("channel", out_chan,              g_io_channel_unref,
						   "window",  g_object_ref (window), g_object_unref,
						   NULL);

	g_child_watch_add_full (G_PRIORITY_DEFAULT,
				pid,
				watch_cb,
				channel_and_window,
				sb_callback_data_free);
}

void
selection_changed_cb (GtkFileChooser* chooser,
		      GtkWidget     * window)
{
	gchar* path = gtk_file_chooser_get_filename (chooser);
	sb_window_open (window,
			path);
	g_free (path);
}

int
main (int   argc,
      char**argv)
{
	gtk_init (&argc, &argv);
	gtk_widget_show_all (sb_window_new ());
	gtk_main ();
	return 0;
}

