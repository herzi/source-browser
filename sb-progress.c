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

#include "sb-progress.h"

struct _SbProgressPrivate {
	gulong target;
};

G_DEFINE_TYPE (SbProgress, sb_progress, GTK_TYPE_PROGRESS_BAR);

static void
sb_progress_init (SbProgress* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_PROGRESS,
						      SbProgressPrivate);

	sb_progress_set_target (self, 0);
}

static void
sb_progress_class_init (SbProgressClass* self_class)
{
	g_type_class_add_private (self_class, sizeof (SbProgressPrivate));
}

GtkWidget*
sb_progress_new (void)
{
	return g_object_new (SB_TYPE_PROGRESS, NULL);
}

gulong
sb_progress_get_target (SbProgress const* self)
{
	g_return_val_if_fail (SB_IS_PROGRESS (self), 0);

	return self->_private->target;
}

void
sb_progress_set_target (SbProgress* self,
			gulong      target)
{
	g_return_if_fail (SB_IS_PROGRESS (self));

	self->_private->target = target;

	// FIXME: g_object_notify (G_OBJECT (self), target);
}

