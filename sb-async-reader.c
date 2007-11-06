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

#include "sb-async-reader.h"

// FIXME: remove this include
#include "sb-display.h"

struct _SbAsyncReaderPrivate {
	guint       io_tag;
	GString   * buffer;
};

enum {
	READ_LINE,
	N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

G_DEFINE_TYPE (SbAsyncReader, sb_async_reader, GFC_TYPE_READER);

static void
sb_async_reader_init (SbAsyncReader* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_ASYNC_READER,
						      SbAsyncReaderPrivate);

	self->_private->buffer = g_string_sized_new (0);
}

static gboolean
io_watch_cb (GIOChannel  * channel,
	     GIOCondition  condition,
	     gpointer      data)
{
	GIOStatus state = G_IO_STATUS_NORMAL;
	SbAsyncReader* self = SB_ASYNC_READER (data);
	gunichar read = 0;

	while (state == G_IO_STATUS_NORMAL) {
		gchar buffer[512];
		gsize delim = 0;

		// FIXME: add GError here
		state = g_io_channel_read_line_string (channel, self->_private->buffer, &delim, NULL);
		switch (state) {
		case G_IO_STATUS_NORMAL:
			g_string_set_size (self->_private->buffer, delim);
			g_signal_emit_by_name (self,
					       "read-line",
					       self->_private->buffer->str); // FIXME: emit by signal id
			g_string_set_size (self->_private->buffer, 0);
			break;
		case G_IO_STATUS_AGAIN:
			/* no data right now... try again later */
			break;
		case G_IO_STATUS_ERROR:
		case G_IO_STATUS_EOF:
			// FIXME: call g_source_remove() here instead of flush()?
			sb_async_reader_set_io_tag (self,
						    0);
			return FALSE;
		}
	}

	return TRUE;
}

static void
reader_constructed (GObject* object)
{
	SbAsyncReader* self = SB_ASYNC_READER (object);

	if (G_OBJECT_CLASS (sb_async_reader_parent_class)->constructed) {
		G_OBJECT_CLASS (sb_async_reader_parent_class)->constructed (object);
	}

	sb_async_reader_set_io_tag (self,
				    g_io_add_watch (gfc_reader_get_channel (GFC_READER (self)),
						    G_IO_IN,
						    io_watch_cb,
						    self));
}

static void
reader_finalize (GObject* object)
{
	SbAsyncReader* self = SB_ASYNC_READER (object);

	g_string_free (self->_private->buffer, TRUE);

	G_OBJECT_CLASS (sb_async_reader_parent_class)->finalize (object);
}

static void
sb_async_reader_class_init (SbAsyncReaderClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->constructed  = reader_constructed;
	object_class->finalize     = reader_finalize;

	signals[READ_LINE] = g_signal_new ("read-line",
					   SB_TYPE_ASYNC_READER,
					   0, 0,
					   NULL, NULL,
					   g_cclosure_marshal_VOID__STRING,
					   G_TYPE_NONE,
					   1,
					   G_TYPE_STRING | G_SIGNAL_TYPE_STATIC_SCOPE);

	g_type_class_add_private (self_class, sizeof (SbAsyncReaderPrivate));
}

SbAsyncReader*
sb_async_reader_new (gint input_fd)
{
	return g_object_new (SB_TYPE_ASYNC_READER,
			     "file-descriptor", input_fd,
			     NULL);
}

guint
sb_async_reader_get_io_tag (SbAsyncReader const* self)
{
	g_return_val_if_fail (SB_IS_ASYNC_READER (self), 0);

	return self->_private->io_tag;
}

void
sb_async_reader_set_io_tag (SbAsyncReader* self,
			    guint          io_tag)
{
	g_return_if_fail (SB_IS_ASYNC_READER (self));

	self->_private->io_tag = io_tag;
}

void
sb_reader_flush (SbAsyncReader* reader)
{
	guint       io_handler = sb_async_reader_get_io_tag (reader);

	if (G_UNLIKELY (!io_handler)) {
		return;
	}

	g_source_remove (io_handler);

	io_watch_cb (gfc_reader_get_channel (GFC_READER (reader)),
		     G_IO_IN,
		     reader); // parse trailing lines
}

