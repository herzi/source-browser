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
	gint        file_descriptor;
	guint       io_tag;
	GIOChannel* channel;
	GString   * buffer;
};

enum {
	PROP_0,
	PROP_FD
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

static void
reader_finalize (GObject* object)
{
	SbAsyncReader* self = SB_ASYNC_READER (object);

	g_string_free (self->_private->buffer, TRUE);

	G_OBJECT_CLASS (sb_async_reader_parent_class)->finalize (object);
}

static void
reader_get_property (GObject   * object,
		     guint       prop_id,
		     GValue    * value,
		     GParamSpec* pspec)
{
	SbAsyncReader* self = SB_ASYNC_READER (object);

	switch (prop_id) {
	case PROP_FD:
		g_value_set_int (value, self->_private->file_descriptor);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
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
			sb_async_reader_set_io_tag (self,
						    0);
			return FALSE;
		}
	}

	return TRUE;
}

static void
reader_set_property (GObject     * object,
		     guint         prop_id,
		     GValue const* value,
		     GParamSpec  * pspec)
{
	SbAsyncReader* self = SB_ASYNC_READER (object);

	switch (prop_id) {
	case PROP_FD:
		self->_private->file_descriptor = g_value_get_int (value);
		g_object_notify (object, "file-descriptor");
		// FIXME: move the channel stuff into the constructed() function
		self->_private->channel = g_io_channel_unix_new (self->_private->file_descriptor);
		g_io_channel_set_flags (self->_private->channel, G_IO_FLAG_NONBLOCK, NULL); // FIXME: return value and GError
		g_io_channel_set_close_on_unref (self->_private->channel, TRUE);

		sb_async_reader_set_io_tag (self,
					    g_io_add_watch (sb_async_reader_get_channel (self),
							    G_IO_IN,
							    io_watch_cb,
							    self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
sb_async_reader_class_init (SbAsyncReaderClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize     = reader_finalize;
	object_class->get_property = reader_get_property;
	object_class->set_property = reader_set_property;

	g_object_class_install_property (object_class, PROP_FD,
					 g_param_spec_int ("file-descriptor", "file-descriptor", "file-descriptor",
							   0, G_MAXINT, 0,
							   G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

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

GIOChannel*
sb_async_reader_get_channel (SbAsyncReader const* self)
{
	g_return_val_if_fail (SB_IS_ASYNC_READER (self), NULL);

	return self->_private->channel;
}

gint
sb_async_reader_get_fd (SbAsyncReader const* self)
{
	// FIXME: remove this function, we shouldn't expose this
	g_return_val_if_fail (SB_IS_ASYNC_READER (self), 0);

	return self->_private->file_descriptor;
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
	GIOChannel* channel;
	guint       io_handler = sb_async_reader_get_io_tag (reader);

	if (G_UNLIKELY (!io_handler)) {
		return;
	}

	g_source_remove (io_handler);

	channel = sb_async_reader_get_channel (reader);
	io_watch_cb (channel, G_IO_IN, reader); // parse trailing lines
}

