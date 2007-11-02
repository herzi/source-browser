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
#include "sb-progress.h"
#include <glib/gi18n.h>

struct _SbWindowPrivate {
	GtkWidget* progress;
	GtkWidget* status;
#ifdef HAVE_PLATFORM_OSX
	GtkWidget* quit_item;
#endif
};

G_DEFINE_TYPE (SbWindow, sb_window, GTK_TYPE_WINDOW);

static void
chooser_selection_changed_cb (GtkFileChooser* chooser,
			      GtkWidget     * window)
{
	gchar* path = gtk_file_chooser_get_filename (chooser);
	sb_window_open (window,
			path);
	g_free (path);
}

static void
display_load_started_cb (SbDisplay* display,
			 GtkWidget* window)
{
	gtk_widget_show (sb_window_get_status (window));
	sb_progress_set_target (SB_PROGRESS (sb_window_get_status (window)),
				sb_display_get_n_lines (SB_DISPLAY (sb_window_get_display (window))));

	// FIXME: this is a bug in GtkTextView (it doesn't swallow the trailing \n)
	sb_progress_set_status (SB_PROGRESS (sb_window_get_status (window)), 1);
}

static void
display_load_progress_cb (SbDisplay* display,
			  gint       lines,
			  GtkWidget* window)
{
	sb_progress_advance (SB_PROGRESS (sb_window_get_status (window)),
			     lines);
}

static void
display_load_done_cb (SbDisplay* display,
		      SbWindow * self)
{
	gtk_widget_hide (sb_window_get_status (GTK_WIDGET (self)));
}

static void
sb_window_init (SbWindow* self)
{
	GtkWidget* result = GTK_WIDGET (self);
	GtkWidget* vbox   = gtk_vbox_new (FALSE, 6);
	GtkWidget* chooser;
	GtkWidget* display;
	GtkWidget* scrolled;

	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_WINDOW,
						      SbWindowPrivate);

	// FIXME: perform in class_init
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
			  G_CALLBACK (chooser_selection_changed_cb), result);
	gtk_box_pack_start (GTK_BOX (vbox),
			    chooser,
			    FALSE,
			    FALSE,
			    0);

	scrolled = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start_defaults (GTK_BOX (vbox),
				     scrolled);

	display = sb_display_new ();
	g_signal_connect  (display, "load-started",
			   G_CALLBACK (display_load_started_cb), result);
	g_signal_connect  (display, "load-progress",
			   G_CALLBACK (display_load_progress_cb), result);
	g_signal_connect  (display, "load-done",
			   G_CALLBACK (display_load_done_cb), result);
	gtk_container_add (GTK_CONTAINER (scrolled),
			   display);

	self->_private->status = gtk_statusbar_new ();
	gtk_widget_show (self->_private->status);
	gtk_box_pack_start (GTK_BOX (vbox),
			    self->_private->status,
			    FALSE,
			    FALSE,
			    0);

	self->_private->progress = sb_progress_new ();
	gtk_box_pack_start (GTK_BOX (self->_private->status),
			    self->_private->progress,
			    FALSE,
			    FALSE,
			    0);

#ifdef HAVE_PLATFORM_OSX
	self->_private->quit_item = gtk_menu_item_new_with_label ("Quit");
	g_signal_connect_swapped (self->_private->quit_item, "activate",
				  G_CALLBACK (gtk_widget_destroy), self);
	ige_mac_menu_set_quit_menu_item (self->_private->quit_item);
#endif
}

static void
window_destroy (GtkObject* object)
{
#ifdef HAVE_PLATFORM_OSX
	SbWindow* self = SB_WINDOW (object);

	if (self->_private->quit_item) {
		gtk_widget_destroy (self->_private->quit_item);
		self->_private->quit_item = NULL;
	}
#endif

	GTK_OBJECT_CLASS (sb_window_parent_class)->destroy (object);
}

static void
sb_window_class_init (SbWindowClass* self_class)
{
	GtkObjectClass* gtk_object_class = GTK_OBJECT_CLASS (self_class);

	gtk_object_class->destroy = window_destroy;

	g_type_class_add_private (self_class, sizeof (SbWindowPrivate));
}

GtkWidget*
sb_window_new (void)
{
	return g_object_new (SB_TYPE_WINDOW, NULL);
}

GtkWidget*
sb_window_get_display (GtkWidget* self)
{
	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	return gtk_bin_get_child (GTK_BIN (sb_window_get_swin (self)));
}

static GtkWidget*
sb_window_get_nth_in_box (GtkWidget* self,
			  guint      n)
{
	GtkWidget* result;
	GList    * children;

	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	children = gtk_container_get_children (GTK_CONTAINER (sb_window_get_vbox (self)));
	result = g_list_nth_data (children, n);
	g_list_free (children);

	return result;
}

GtkWidget*
sb_window_get_status (GtkWidget* self)
{
	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	return SB_WINDOW (self)->_private->progress;
}

GtkWidget*
sb_window_get_swin (GtkWidget* self)
{
	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	return sb_window_get_nth_in_box (self, 1);
}

GtkWidget*
sb_window_get_vbox (GtkWidget* self)
{
	g_return_val_if_fail (GTK_IS_WINDOW (self), NULL);

	return gtk_bin_get_child (GTK_BIN (self));
}

void
sb_window_open (GtkWidget  * window,
		gchar const* path)
{
	GError       * error = NULL;

	sb_display_load_path (SB_DISPLAY (sb_window_get_display (window)),
			      path,
			      &error);

	if (error) {
		// FIXME: open popup
		g_warning ("%s", error->message);
		g_error_free (error);
		return;
	}
}

