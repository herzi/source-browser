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

#ifndef SB_ASYNC_READER_H
#define SB_ASYNC_READER_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _SbAsyncReader        SbAsyncReader;
typedef struct _SbAsyncReaderPrivate SbAsyncReaderPrivate;
typedef struct _SbAsyncReaderClass   SbAsyncReaderClass;

#define SB_TYPE_ASYNC_READER         (sb_async_reader_get_type ())
#define SB_ASYNC_READER(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_ASYNC_READER, SbAsyncReader))

GType          sb_async_reader_get_type (void);
SbAsyncReader* sb_async_reader_new      (gint input_fd);

struct _SbAsyncReader {
	GObject               base_instance;
	SbAsyncReaderPrivate* _private;
};

struct _SbAsyncReaderClass {
	GObjectClass          base_class;
};

G_END_DECLS

#endif /* !SB_ASYNC_READER_H */
