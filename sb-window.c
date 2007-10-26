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
	GtkTextBuffer* buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (sb_window_get_display (window)));
	sb_progress_set_target (SB_PROGRESS (sb_window_get_status (window)),
				gtk_text_buffer_get_line_count (buffer));

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

GtkWidget*
sb_window_new (void)
{
	GtkWidget* result = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget* vbox   = gtk_vbox_new (FALSE, 6);
	GtkWidget* chooser;
	GtkWidget* display;
	GtkWidget* scrolled;
	GtkWidget* status;

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
	gtk_container_add (GTK_CONTAINER (scrolled),
			   display);

	status = sb_progress_new ();
	gtk_box_pack_start (GTK_BOX (vbox),
			    status,
			    FALSE,
			    FALSE,
			    0);

	return result;
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

	return sb_window_get_nth_in_box (self, 2);
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

