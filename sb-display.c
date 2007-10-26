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

#include "sb-display.h"

struct _SbDisplayPrivate {
	guint io_handler;
};

enum {
	LOAD_STARTED,
	LOAD_PROGRESS,
	// LOAD_DONE,
	// LOAD_CANCELLED,
	N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

G_DEFINE_TYPE (SbDisplay, sb_display, GTK_TYPE_TEXT_VIEW);

static void
sb_display_init (SbDisplay* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_DISPLAY,
						      SbDisplayPrivate);
}

static void
display_finalize (GObject* object)
{
	G_OBJECT_CLASS (sb_display_parent_class)->finalize (object);
}

static void
sb_display_class_init (SbDisplayClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = display_finalize;

	g_type_class_add_private (self_class, sizeof (SbDisplayPrivate));

	signals[LOAD_STARTED] = g_signal_new ("load-started",
					      SB_TYPE_DISPLAY,
					      0, 0,
					      NULL, NULL,
					      g_cclosure_marshal_VOID__VOID,
					      G_TYPE_NONE,
					      0);
	signals[LOAD_PROGRESS] = g_signal_new ("load-progress",
					       SB_TYPE_DISPLAY,
					       0, 0,
					       NULL, NULL,
					       g_cclosure_marshal_VOID__INT,
					       G_TYPE_NONE,
					       1, G_TYPE_INT);
}

GtkWidget*
sb_display_new (void)
{
	return g_object_new (SB_TYPE_DISPLAY, NULL);
}

guint
sb_display_get_io_handler (SbDisplay const* self)
{
	g_return_val_if_fail (SB_IS_DISPLAY (self), 0);

	return self->_private->io_handler;
}

void
sb_display_set_io_handler (SbDisplay* self,
			   guint      io_handler)
{
	g_return_if_fail (SB_IS_DISPLAY (self));

	if (self->_private->io_handler == io_handler) {
		return;
	}

	if (self->_private->io_handler) {
		//g_source_remove (self->_private->io_handler);
	}

	self->_private->io_handler = io_handler;

	// FIXME: g_object_notify (G_OBJECT (self), "io-handler");
}

void
sb_display_load_path (SbDisplay  * self,
		      gchar const* path,
		      GError     **error)
{
	GtkTextBuffer* buffer;
	GMappedFile  * file;

	file = g_mapped_file_new (path, FALSE, error);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (self));

	gtk_text_buffer_set_text (buffer,
				  g_mapped_file_get_contents (file),
				  g_mapped_file_get_length (file));

	g_signal_emit (self, signals[LOAD_STARTED], 0);

	g_mapped_file_free (file);

	load_history (self,
		      path);

	// FIXME: disable loading of new files until the history is loaded
	// FIXME: make history loading cancellable
}

