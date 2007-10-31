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
	GtkLabel   * label;
	SbReference* reference;
};

enum {
	PROP_0,
	PROP_REFERENCE
};

G_DEFINE_TYPE (SbReferenceLabel, sb_reference_label, GTK_TYPE_EVENT_BOX);

static void
sb_reference_label_init (SbReferenceLabel* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_REFERENCE_LABEL,
						      SbReferenceLabelPrivate);

	self->_private->label = GTK_LABEL (gtk_label_new (""));
	gtk_widget_show (GTK_WIDGET (self->_private->label));
	gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET (self->_private->label));
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
		g_value_set_object (value, sb_reference_label_get_reference (self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static inline void
update_background (SbReferenceLabel* self)
{
	gchar const* name = sb_revision_get_name (sb_reference_get_revision (self->_private->reference));
	guint        hash = name ? g_str_hash (name) : 0;
	GdkColor colors[] = {
		{0, 0xfcfc, 0xe9e9, 0x4f4f}, // butter
		{0, 0xfcfc, 0xafaf, 0x3e3e}, // orange
		{0, 0xe9e9, 0xb9b9, 0x6e6e}, // chocolate
		{0, 0x8a8a, 0xe2e2, 0x3434}, // chameleon
		{0, 0x7272, 0x9f9f, 0xcfcf}, // sky blue
		{0, 0xadad, 0x7f7f, 0xa8a8}, // plum
		{0, 0xefef, 0x2929, 0x2929}, // scarlet red
		{0, 0xeeee, 0xeeee, 0xecec}  // aluminium
	};

	gtk_widget_modify_bg (GTK_WIDGET (self),
			      GTK_STATE_NORMAL,
			      &colors[hash & 0x7]);
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

		g_object_set (self->_private->label, "label", sb_revision_get_name (sb_reference_get_revision (self->_private->reference)), NULL);
		update_background (self);
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

SbReference*
sb_reference_label_get_reference (SbReferenceLabel const* self)
{
	g_return_val_if_fail (SB_IS_REFERENCE_LABEL (self), NULL);

	return self->_private->reference;
}

