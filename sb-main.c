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

static GtkWidget* tview = NULL;

static void
watch_cb (GPid pid,
	  gint status,
	  gpointer unused)
{
	g_spawn_close_pid (pid);
}

static void
load_history (GdkScreen  * screen,
	      gchar const* file_path)
{
	gchar* working_folder = g_path_get_dirname (file_path);
	gchar* argv[] = {
		"git-blame",
		"--interactive",
		NULL,
		NULL
	};
	GPid pid = 0;
	gint out_fd = 0;
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
	g_free (argv[2]);
	g_free (working_folder);

	g_child_watch_add (pid,
			   watch_cb,
			   NULL);
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

	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}

