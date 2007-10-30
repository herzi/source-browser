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

G_DEFINE_TYPE (SbAnnotations, sb_annotations, GTK_TYPE_LAYOUT);

static void
sb_annotations_init (SbAnnotations* self)
{
	GtkWidget* result = GTK_WIDGET (self);
	GtkWidget* label  = gtk_label_new ("Annotation");
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.0);
	gtk_widget_show (label);
	gtk_layout_put (GTK_LAYOUT (result),
			label,
			10,10);
	gtk_widget_set_size_request (result, 100, 100);
}

static void
sb_annotations_class_init (SbAnnotationsClass* self_class)
{}

GtkWidget*
sb_annotations_new (void)
{
	return g_object_new (SB_TYPE_ANNOTATIONS, NULL);
}

