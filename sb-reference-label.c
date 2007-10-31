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

#include "sb-reference-label.h"

struct _SbReferenceLabelPrivate {
	SbReference* reference;
};

enum {
	PROP_0,
	PROP_REFERENCE
};

G_DEFINE_TYPE (SbReferenceLabel, sb_reference_label, GTK_TYPE_LABEL);

static void
sb_reference_label_init (SbReferenceLabel* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_REFERENCE_LABEL,
						      SbReferenceLabelPrivate);
}

static void
label_finalize (GObject* object)
{
	SbReferenceLabel* self = SB_REFERENCE_LABEL (object);

	if (G_LIKELY (self->_private->reference)) {
		g_object_unref (self->_private->reference);
	}

	G_OBJECT_CLASS (sb_reference_label_parent_class)->finalize (object);
}

static void
label_get_property (GObject   * object,
		    guint       prop_id,
		    GValue    * value,
		    GParamSpec* pspec)
{
	SbReferenceLabel* self = SB_REFERENCE_LABEL (object);

	switch (prop_id) {
	case PROP_REFERENCE:
		g_value_set_object (value, self->_private->reference);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
label_set_property (GObject     * object,
		    guint         prop_id,
		    GValue const* value,
		    GParamSpec  * pspec)
{
	SbReferenceLabel* self = SB_REFERENCE_LABEL (object);

	switch (prop_id) {
	case PROP_REFERENCE:
		g_return_if_fail (!self->_private->reference);
		// FIXME: drop the cast once depending on glib 2.14
		self->_private->reference = SB_REFERENCE (g_value_dup_object (value));
		g_object_notify (object, "reference");

		g_object_set (self, "label", sb_revision_get_name (sb_reference_get_revision (self->_private->reference)), NULL);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
sb_reference_label_class_init (SbReferenceLabelClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize     = label_finalize;
	object_class->get_property = label_get_property;
	object_class->set_property = label_set_property;

	g_object_class_install_property (object_class, PROP_REFERENCE,
					 g_param_spec_object ("reference", "reference", "reference",
							      SB_TYPE_REFERENCE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_type_class_add_private (self_class, sizeof (SbReferenceLabelPrivate));
}

GtkWidget*
sb_reference_label_new (SbReference* reference)
{
	return g_object_new (SB_TYPE_REFERENCE_LABEL,
			     "reference", reference,
			     NULL);
}

