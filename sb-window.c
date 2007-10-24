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

#include "sb-window.h"

#include "sb-display.h"
#include "sb-main.h"
#include <glib/gi18n.h>

GtkWidget*
sb_window_new (void)
{
	GtkWidget* result = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget* vbox   = gtk_vbox_new (FALSE, 6);
	GtkWidget* chooser;
	GtkWidget* display;
	GtkWidget* scrolled;

	g_signal_connect (result, "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_default_size (GTK_WINDOW (result),
				     400, 300);
	gtk_window_set_title        (GTK_WINDOW (result),
				     _("Source Browser"));

	gtk_container_add (GTK_CONTAINER (result),
			   vbox);

	chooser = gtk_file_chooser_button_new (_("Choose File"),
					       GTK_FILE_CHOOSER_ACTION_OPEN);
	g_signal_connect (chooser, "selection-changed",
			  G_CALLBACK (selection_changed_cb), NULL);
	gtk_box_pack_start (GTK_BOX (vbox),
			    chooser,
			    FALSE,
			    FALSE,
			    0);

	scrolled = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start_defaults (GTK_BOX (vbox),
				     scrolled);

	display = sb_display_new ();
	gtk_container_add (GTK_CONTAINER (scrolled),
			   display);

	return result;
}

GtkWidget*
sb_window_get_display (GtkWidget* self)
{
	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	return gtk_bin_get_child (GTK_BIN (sb_window_get_swin (self)));
}

GtkWidget*
sb_window_get_swin (GtkWidget* self)
{
	GtkWidget* result;
	GList    * children;

	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	children = gtk_container_get_children (GTK_CONTAINER (sb_window_get_vbox (self)));
	result = g_list_nth_data (children, 1);
	g_list_free (children);

	return result;
}

GtkWidget*
sb_window_get_vbox (GtkWidget* self)
{
	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	return gtk_bin_get_child (GTK_BIN (self));
}

