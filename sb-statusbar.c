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

#include "sb-statusbar.h"

static void
statusbar_add_cb (GtkContainer* statusbar,
		  GtkWidget   * child,
		  GtkSizeGroup* group)
{
	gtk_size_group_add_widget (group, child);
}

static void
statusbar_remove_cb (GtkContainer* statusbar,
		     GtkWidget   * child,
		     GtkSizeGroup* group)
{
	if (!GPOINTER_TO_INT (g_object_get_data (G_OBJECT (statusbar), "SbStatusbarDestroyed"))) {
		gtk_size_group_remove_widget (group, child);
	}
}

static void
statusbar_destroy_cb (GtkObject   * object,
		      GtkSizeGroup* group)
{
	GList* children = gtk_container_get_children (GTK_CONTAINER (object));
	GList* iter;
	for (iter = children; iter; iter = iter->next) {
		statusbar_remove_cb (GTK_CONTAINER (object), iter->data, group);
	}
	g_object_set_data (G_OBJECT (object), "SbStatusbarDestroyed", GINT_TO_POINTER (TRUE));
	g_list_free (children);
}

GtkWidget*
sb_status_bar_new (void)
{
	GtkWidget* result = gtk_statusbar_new ();
	GtkSizeGroup* group = gtk_size_group_new (GTK_SIZE_GROUP_BOTH);
	GList* children = gtk_container_get_children (GTK_CONTAINER (result));
	GList* iter;
	g_signal_connect_data (result, "add",
			       G_CALLBACK (statusbar_add_cb), group,
			       (GClosureNotify)g_object_unref,
			       0);
	g_signal_connect_data (result, "remove",
			       G_CALLBACK (statusbar_remove_cb), g_object_ref (group),
			       (GClosureNotify)g_object_unref,
			       0);
	g_signal_connect_data (result, "destroy",
			       G_CALLBACK (statusbar_destroy_cb),
			       g_object_ref (group),
			       (GClosureNotify)g_object_unref,
			       0);
	for (iter = children; iter; iter = iter->next) {
		statusbar_add_cb (GTK_CONTAINER (result), iter->data, group);
	}
	g_list_free (children);
	return result;
}

