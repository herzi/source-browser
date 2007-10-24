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

#include "sb-display.h"
#include "sb-window.h"

#include <glib/gi18n.h>

static gint lines_read = 0;
static guint io = 0;

static gboolean io_watch_cb (GIOChannel  * channel,
			     GIOCondition  condition,
			     gpointer      unused);

static void
watch_cb (GPid pid,
	  gint status_,
	  gpointer data)
{
	GIOChannel* channel = ((gpointer*)data)[0];
	GtkWidget * window  = ((gpointer*)data)[1];
	g_print ("pre-done.\n");
	while (io_watch_cb (channel, G_IO_IN, window)) {} // FIXME: finish
	//g_source_remove (io);
	io = 0;
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
	GtkWidget* window = GTK_WIDGET (data);

	if (G_UNLIKELY (!string)) {
		string = g_string_new ("");
	}

	if (condition != 1)
	g_print ("%d\n", condition);

	state = g_io_channel_read_unichar (channel, &read, NULL);
	if (G_LIKELY (read != '\n')) {
		g_string_append_unichar (string, read);
	} else {
		if (!revision) {
			// "<40-byte hex sha1> <sourceline> <resultline> <num_lines>"
			gchar** words = g_strsplit (string->str, " ", -1);
			revision = g_strdup (words[0]);
			lines_read += atoi (words[3]);
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (sb_window_get_status (window)),
						       1.0 * lines_read / gtk_text_buffer_get_line_count (gtk_text_view_get_buffer (GTK_TEXT_VIEW (sb_window_get_display (window)))));
			g_strfreev (words);
			gchar* message = g_strdup_printf (_("%d / %d"), lines_read, gtk_text_buffer_get_line_count (gtk_text_view_get_buffer (GTK_TEXT_VIEW (sb_window_get_display (window)))));
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (sb_window_get_status (window)),
						   message);
			g_free (message);
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
		return FALSE;
	}
}

static void
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

	lines_read = 1; // FIXME: this is a bug in GtkTextView (it doesn't swallow the trailing \n)

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
			     NULL); // FIXME: error, pipes, flags
	GIOChannel* out_chan = g_io_channel_unix_new (out_fd);
	g_free (argv[2]);
	g_free (working_folder);

	io = g_io_add_watch (out_chan,
			     G_IO_IN | G_IO_PRI,
			     io_watch_cb,
			     window);

	channel_and_window = g_new (gpointer, 2);
	channel_and_window[0] = out_chan;
	channel_and_window[1] = window;

	g_child_watch_add_full (G_PRIORITY_DEFAULT,
				pid,
				watch_cb,
				channel_and_window,
				g_free);
}

static void
sb_window_open (GtkWidget  * window,
		gchar const* path)
{
	GMappedFile* file;
	GError* error = NULL;
	file = g_mapped_file_new (path, FALSE, &error);
	if (!file) {
		// FIXME: open popup
		g_warning ("%s", error->message);
		g_error_free (error);
		return;
	}

	gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (sb_window_get_display (window))),
				  g_mapped_file_get_contents (file),
				  g_mapped_file_get_length (file));

	g_mapped_file_free (file);

	load_history (window,
		      path);

	// FIXME: disable loading of new files until the history is loaded
	// FIXME: make history loading cancellable
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

