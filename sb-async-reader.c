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

struct _SbAsyncReaderPrivate {
	gint file_descriptor;
};

enum {
	PROP_0,
	PROP_FD
};

G_DEFINE_TYPE (SbAsyncReader, sb_async_reader, G_TYPE_OBJECT);

static void
sb_async_reader_init (SbAsyncReader* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_ASYNC_READER,
						      SbAsyncReaderPrivate);
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

	object_class->set_property = reader_set_property;

	g_object_class_install_property (object_class, PROP_FD,
					 g_param_spec_int ("file-descriptor", "file-descriptor", "file-descriptor",
							   0, G_MAXINT, 0,
							   G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

	g_type_class_add_private (self_class, sizeof (SbAsyncReaderPrivate));
}

SbAsyncReader*
sb_async_reader_new (gint input_fd)
{
	return g_object_new (SB_TYPE_ASYNC_READER,
			     "file-descriptor", input_fd,
			     NULL);
}

