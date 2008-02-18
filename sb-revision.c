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

#include "sb-comparable.h"

struct _SbRevisionPrivate {
	gchar* name;
	gchar* summary;
};

enum {
	PROP_0,
	PROP_NAME
};

static void implement_comparable (SbComparableIface* iface);
G_DEFINE_TYPE_WITH_CODE (SbRevision, sb_revision, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (SB_TYPE_COMPARABLE, implement_comparable););

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
	g_free (self->_private->summary);

	G_OBJECT_CLASS (sb_revision_parent_class)->finalize (object);
}

static void
revision_get_property (GObject   * object,
		       guint       prop_id,
		       GValue    * value,
		       GParamSpec* pspec)
{
	SbRevision* self = SB_REVISION (object);

	switch (prop_id) {
	case PROP_NAME:
		g_value_set_string (value, sb_revision_get_name (self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
revision_set_property (GObject     * object,
		       guint         prop_id,
		       GValue const* value,
		       GParamSpec  * pspec)
{
	SbRevision* self = SB_REVISION (object);

	switch (prop_id) {
	case PROP_NAME:
		self->_private->name = g_value_dup_string (value);
		g_object_notify (object, "name");
		break;
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
	object_class->get_property = revision_get_property;
	object_class->set_property = revision_set_property;

	g_object_class_install_property (object_class,
					 PROP_NAME,
					 g_param_spec_string ("name", "name", "name",
							      NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_type_class_add_private (self_class, sizeof (SbRevisionPrivate));
}

SbRevision*
sb_revision_new (gchar const* name)
{
	return g_object_new (SB_TYPE_REVISION,
			     "name", name,
			     NULL);
}

gchar const*
sb_revision_get_name (SbRevision const* self)
{
	g_return_val_if_fail (SB_IS_REVISION (self), NULL);

	return self->_private->name;
}

gchar const*
sb_revision_get_summary (SbRevision const* self)
{
	g_return_val_if_fail (SB_IS_REVISION (self), NULL);

	return self->_private->summary;
}

void
sb_revision_set_summary (SbRevision * self,
			 gchar const* summary)
{
	g_return_if_fail (SB_IS_REVISION (self));

	if (self->_private->summary == summary) {
		return;
	}

	g_free (self->_private->summary);
	self->_private->summary = g_strdup (summary);

	// FIXME: make a GObject property
	// g_object_notify (G_OBJECT (self), "summary");
}

/* SbComparableIface */
static gboolean
revision_equals (SbComparable const* comparable,
		 SbComparable const* other)
{
	SbRevision* self = SB_REVISION (comparable);
	return SB_IS_REVISION (other) &&
	       !strcmp (self->_private->name, SB_REVISION (other)->_private->name);
}

static guint
revision_hash (SbComparable const* comparable)
{
	SbRevision const* self = SB_REVISION (comparable);

	return g_str_hash (self->_private->name); // FIXME: has also on a repository somehow
}

static void
implement_comparable (SbComparableIface* iface)
{
	iface->equals = revision_equals;
	iface->hash   = revision_hash;
}

