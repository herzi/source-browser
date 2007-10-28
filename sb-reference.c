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

#include "sb-reference.h"

struct _SbReferencePrivate {
	SbRevision* revision;
};

enum {
	PROP_0,
	PROP_REVISION
};

G_DEFINE_TYPE (SbReference, sb_reference, G_TYPE_OBJECT);

static void
sb_reference_init (SbReference* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_REFERENCE,
						      SbReferencePrivate);
}

static void
reference_finalize (GObject* object)
{
	SbReference* self = SB_REFERENCE (object);

	g_object_unref (self->_private->revision);

	G_OBJECT_CLASS (sb_reference_parent_class)->finalize (object);
}

static void
sb_reference_class_init (SbReferenceClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = reference_finalize;

	g_object_class_install_property (object_class, PROP_REVISION,
					 g_param_spec_object ("revision", "revision", "revision",
							      SB_TYPE_REVISION, 0));

	g_type_class_add_private (self_class, sizeof (SbReferencePrivate));
}

SbReference*
sb_reference_new (SbRevision* revision)
{
	return g_object_new (SB_TYPE_REFERENCE,
			     NULL);
}

