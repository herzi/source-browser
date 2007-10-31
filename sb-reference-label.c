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
sb_reference_label_class_init (SbReferenceLabelClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = label_finalize;

	g_object_class_install_property (object_class, PROP_REFERENCE,
					 g_param_spec_object ("reference", "reference", "reference",
							      SB_TYPE_REFERENCE, 0));

	g_type_class_add_private (self_class, sizeof (SbReferenceLabelPrivate));
}

GtkWidget*
sb_reference_label_new (SbReference* reference)
{
	return g_object_new (SB_TYPE_REFERENCE_LABEL,
			     "label", sb_revision_get_name (sb_reference_get_revision (reference)),
			     NULL);
}

