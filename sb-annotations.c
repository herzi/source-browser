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

#include "sb-annotations.h"

#include "sb-reference.h"

struct _SbAnnotationsPrivate {
	GList      * references;
	GtkTextView* text_view;
};

enum {
	PROP_0,
	PROP_REFERENCES,
	PROP_TEXT_VIEW
};

G_DEFINE_TYPE (SbAnnotations, sb_annotations, GTK_TYPE_LAYOUT);

static void
sb_annotations_init (SbAnnotations* self)
{
	GtkWidget* result = GTK_WIDGET (self);
	GtkWidget* label  = gtk_label_new ("Annotation");

	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_ANNOTATIONS,
						      SbAnnotationsPrivate);

	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.0);
	gtk_widget_show (label);
	gtk_layout_put (GTK_LAYOUT (result),
			label,
			10,10);
	gtk_widget_set_size_request (result, 100, 100);
}

static void
annotations_dispose (GObject* object)
{
	SbAnnotations* self = SB_ANNOTATIONS (object);

	sb_annotations_set_references (self, NULL);
	sb_annotations_set_text_view  (self, NULL); // FIXME: should go into destroy()

	G_OBJECT_CLASS (sb_annotations_parent_class)->dispose (object);
}

static void
annotations_set_property (GObject     * object,
			  guint         prop_id,
			  GValue const* value,
			  GParamSpec  * pspec)
{
	SbAnnotations* self = SB_ANNOTATIONS (object);

	switch (prop_id) {
	case PROP_REFERENCES:
		sb_annotations_set_references (self, g_value_get_pointer (value));
		break;
	case PROP_TEXT_VIEW:
		sb_annotations_set_text_view  (self, g_value_get_object (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
sb_annotations_class_init (SbAnnotationsClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->dispose      = annotations_dispose;
	object_class->set_property = annotations_set_property;

	g_object_class_install_property (object_class, PROP_REFERENCES,
					 g_param_spec_pointer ("references", "references", "references",
							       G_PARAM_WRITABLE));
	g_object_class_install_property (object_class, PROP_TEXT_VIEW,
					 g_param_spec_object ("text-view", "text-view", "text-view",
							      GTK_TYPE_TEXT_VIEW, 0));

	g_type_class_add_private (self_class, sizeof (SbAnnotationsClass));
}

GtkWidget*
sb_annotations_new (void)
{
	return g_object_new (SB_TYPE_ANNOTATIONS, NULL);
}

static inline void
update_labels (SbAnnotations* self)
{
	GList* children = gtk_container_get_children (GTK_CONTAINER (self));
	g_list_foreach (children, (GFunc)gtk_object_destroy, NULL);
	g_list_free    (children);

	for (children = self->_private->references; children; children = children->next) {
		GtkWidget* label = gtk_label_new (sb_revision_get_name (sb_reference_get_revision (children->data)));
		gtk_widget_show (label);
		gtk_layout_put (GTK_LAYOUT (self),
				label,
				0,
				19 * (sb_reference_get_current_start (children->data) - 1));
	}
}

void
sb_annotations_set_references (SbAnnotations* self,
			       GList        * references)
{
	g_return_if_fail (SB_IS_ANNOTATIONS (self));

	if (self->_private->references) {
		g_list_foreach (self->_private->references, (GFunc)g_object_unref, NULL);
		g_list_free    (self->_private->references);
		self->_private->references = NULL;
	}

	if (references) {
		self->_private->references = g_list_copy (references);
		g_list_foreach (self->_private->references, (GFunc)g_object_ref, NULL);
	}

	g_object_notify (G_OBJECT (self), "references");

	update_labels (self);
}

void
sb_annotations_set_text_view (SbAnnotations* self,
			      GtkTextView  * text_view)
{
	g_return_if_fail (SB_IS_ANNOTATIONS (self));
	g_return_if_fail (!text_view || GTK_IS_TEXT_VIEW (text_view));

	if (text_view == self->_private->text_view) {
		return;
	}

	if (self->_private->text_view) {
		g_object_unref (self->_private->text_view);
		self->_private->text_view = NULL;
	}

	if (text_view) {
		self->_private->text_view = g_object_ref_sink (self->_private->text_view);
		// FIXME: connect to destroy() and act properly
	}

	g_object_notify (G_OBJECT (self), "text-view");
}

