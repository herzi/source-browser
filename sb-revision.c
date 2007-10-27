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

#include "sb-revision.h"

struct _SbRevisionPrivate {
	gchar* name;
};

G_DEFINE_TYPE (SbRevision, sb_revision, G_TYPE_OBJECT);

static void
sb_revision_init (SbRevision* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_REVISION,
						      SbRevisionPrivate);
}

static void
revision_finalize (GObject* object)
{
	SbRevision* self = SB_REVISION (object);

	g_free (self->_private->name);

	G_OBJECT_CLASS (sb_revision_parent_class)->finalize (object);
}

static void
revision_set_property (GObject     * object,
		       guint         prop_id,
		       GValue const* value,
		       GParamSpec  * pspec)
{
	SbRevision* self = SB_REVISION (object);

	switch (prop_id) {
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
sb_revision_class_init (SbRevisionClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize     = revision_finalize;
	object_class->set_property = revision_set_property;

	g_type_class_add_private (self_class, sizeof (SbRevisionPrivate));
}

SbRevision*
sb_revision_new (void)
{
	return g_object_new (SB_TYPE_REVISION,
			     NULL);
}

