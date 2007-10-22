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

#include <gtk/gtk.h>

#include <glib/gi18n.h>

static GtkWidget* tview  = NULL;
static GtkWidget* status = NULL;
static gint lines_read = 0;

static void
watch_cb (GPid pid,
	  gint status_,
	  gpointer data)
{
	GIOChannel* channel = data;
	GString* string = g_string_new ("");

	gchar* revision = NULL;

	while (G_IO_STATUS_NORMAL == g_io_channel_read_line_string (channel, string, NULL, NULL)) {
		if (!revision) {
			// "<40-byte hex sha1> <sourceline> <resultline> <num_lines>"
			gchar** words = g_strsplit (string->str, " ", -1);
			revision = g_strdup (words[0]);
			lines_read += atoi (words[3]);
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (status),
						       1.0 * lines_read / gtk_text_buffer_get_line_count (gtk_text_view_get_buffer (GTK_TEXT_VIEW (tview))));
			g_strfreev (words);
			gchar* message = g_strdup_printf (_("%d / %d"), lines_read, gtk_text_buffer_get_line_count (gtk_text_view_get_buffer (GTK_TEXT_VIEW (tview))));
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (status),
						   message);
			g_free (message);
		} else if (g_str_has_prefix (string->str, "filename ")) {
			g_free (revision);
			revision = NULL;
#if 0
		} else {
			// FIXME: meta-information about the commit
			g_debug ("Got unknown line: %s",
				 string->str);
#endif
		}
	}

	g_string_free (string, TRUE);
	g_spawn_close_pid (pid);
}

static void
load_history (GdkScreen  * screen,
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

	lines_read = 1;

	argv[2] = g_path_get_basename (file_path);
	gdk_spawn_on_screen_with_pipes (screen,
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

	g_child_watch_add (pid,
			   watch_cb,
			   out_chan);
}

static void
selection_changed_cb (GtkFileChooser* chooser)
{
	GMappedFile* file;
	GError* error = NULL;
	gchar* path;

	path = gtk_file_chooser_get_filename (chooser);
	file = g_mapped_file_new (path, FALSE, &error);
	if (!file) {
		// FIXME: open popup
		g_warning ("%s", error->message);
		g_error_free (error);
		g_free (path);
		return;
	}

	gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (tview)),
				  g_mapped_file_get_contents (file),
				  g_mapped_file_get_length (file));

	g_mapped_file_free (file);

	load_history (gtk_widget_get_screen (GTK_WIDGET (chooser)),
		      path);
	g_free (path);

	// FIXME: disable loading of new files until the history is loaded
	// FIXME: make history loading cancellable
}

int
main (int   argc,
      char**argv)
{
	GtkWidget* window;
	GtkWidget* chooser;
	GtkWidget* vbox;
	GtkWidget* scrolled;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_default_size (GTK_WINDOW (window),
				     400, 300);
	gtk_window_set_title        (GTK_WINDOW (window),
				     _("Source Browser"));

	vbox = gtk_vbox_new (FALSE, 6);
	gtk_container_add (GTK_CONTAINER (window),
			   vbox);

	chooser = gtk_file_chooser_button_new (_("Choose File"),
					       GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_box_pack_start (GTK_BOX (vbox),
			    chooser,
			    FALSE,
			    FALSE,
			    0);

	g_signal_connect (chooser, "selection-changed",
			  G_CALLBACK (selection_changed_cb), NULL);

	scrolled = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start_defaults (GTK_BOX (vbox),
				     scrolled);

	tview = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER (scrolled),
			   tview);

	status = gtk_progress_bar_new ();
	gtk_box_pack_start (GTK_BOX (vbox),
			    status,
			    FALSE,
			    FALSE,
			    0);

	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}

