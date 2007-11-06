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

G_DEFINE_TYPE (SbAsyncReader, sb_async_reader, GFC_TYPE_READER);

static void
sb_async_reader_init (SbAsyncReader* self)
{}

static void
sb_async_reader_class_init (SbAsyncReaderClass* self_class)
{}

SbAsyncReader*
sb_async_reader_new (gint input_fd)
{
	return g_object_new (SB_TYPE_ASYNC_READER,
			     "file-descriptor", input_fd,
			     NULL);
}

void
sb_reader_flush (SbAsyncReader* self)
{
	g_return_if_fail (SB_IS_ASYNC_READER (self));

	gfc_reader_flush (GFC_READER (self));
}

