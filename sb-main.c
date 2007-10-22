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

static void
selection_changed_cb (GtkFileChooser* chooser)
{
	g_print ("file selected: %s\n", gtk_file_chooser_get_filename (chooser));
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

	scrolled = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start_defaults (GTK_BOX (vbox),
				     scrolled);

	g_signal_connect (chooser, "selection-changed",
			  G_CALLBACK (selection_changed_cb), NULL);

	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}

