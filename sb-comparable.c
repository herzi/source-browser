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

#include "sb-comparable.h"

#include "gobject-helpers.h"

G_DEFINE_IFACE (SbComparable, sb_comparable, G_TYPE_INTERFACE);

gboolean
sb_comparable_equals (SbComparable const* self,
		      SbComparable const* other)
{
	SbComparableIface* iface;

	g_return_val_if_fail (SB_IS_COMPARABLE (self), FALSE);
	g_return_val_if_fail (SB_IS_COMPARABLE (other), FALSE);

	iface = SB_COMPARABLE_GET_IFACE (self);
	if (!iface->equals) {
		g_warning ("%s doesn't implement SbComparableIface::equals()",
			   G_OBJECT_TYPE_NAME (self));
		return FALSE;
	}

	return iface->equals (self, other);
}

guint
sb_comparable_hash (SbComparable const* self)
{
	SbComparableIface* iface;

	g_return_val_if_fail (SB_IS_COMPARABLE (self), 0);

	iface = SB_COMPARABLE_GET_IFACE (self);
	if (!iface->hash) {
		g_warning ("%s doesn't implement SbComparableIface::hash()",
			   G_OBJECT_TYPE_NAME (self));
		return 0;
	}

	return iface->hash (self);
}

