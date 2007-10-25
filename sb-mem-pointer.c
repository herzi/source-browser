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

#include "sb-mem-pointer.h"

struct _SbMemPointer {
	gpointer       data;
	GDestroyNotify destroy;
};

SbMemPointer*
sb_mem_pointer_new  (gpointer        data,
		     GDestroyNotify  destroy)
{
	SbMemPointer* self = g_slice_new (SbMemPointer);
	self->data    = data;
	self->destroy = destroy;
	return self;
}

void
sb_mem_pointer_free (SbMemPointer  * self)
{
	if (self->destroy) {
		self->destroy (self->data);
	}
	g_slice_free (SbMemPointer, self);
}

gpointer
sb_mem_pointer_get_data (SbMemPointer const* self)
{
	g_return_val_if_fail (self, NULL);

	return self->data;
}

