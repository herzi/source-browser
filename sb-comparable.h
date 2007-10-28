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

#ifndef SB_COMPARABLE_H
#define SB_COMPARABLE_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _SbComparable      SbComparable;
typedef struct _SbComparableIface SbComparableIface;

#define SB_TYPE_COMPARABLE         (sb_comparable_get_type ())
#define SB_COMPARABLE(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_COMPARABLE, SbComparable))
#define SB_IS_COMPARABLE(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), SB_TYPE_COMPARABLE))
#define SB_COMPARABLE_GET_CLASS(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), SB_TYPE_COMPARABLE, SbComparableIface))

GType sb_comparable_get_type (void);

struct _SbComparableIface {
	GTypeInterface base_interface;

	/* vtable */
};

G_END_DECLS

#endif /* !SB_COMPARABLE_H */
