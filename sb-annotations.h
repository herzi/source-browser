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

#ifndef SB_ANNOTATIONS_H
#define SB_ANNOTATIONS_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _SbAnnotations        SbAnnotations;
typedef struct _SbAnnotationsPrivate SbAnnotationsPrivate;
typedef struct _SbAnnotationsClass   SbAnnotationsClass;

#define SB_TYPE_ANNOTATIONS         (sb_annotations_get_type ())
#define SB_ANNOTATIONS(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_ANNOTATIONS, SbAnnotations))
#define SB_IS_ANNOTATIONS(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), SB_TYPE_ANNOTATIONS))

GtkWidget* sb_annotations_new            (void);
void       sb_annotations_set_references (SbAnnotations* self,
					  GList        * references);
void       sb_annotations_set_text_view  (SbAnnotations* self,
					  GtkTextView  * text_view);

struct _SbAnnotations {
	GtkLayout             base_instance;
	SbAnnotationsPrivate* _private;
};

struct _SbAnnotationsClass {
	GtkLayoutClass        base_class;
};

G_END_DECLS

#endif /* !SB_ANNOTATIONS_H */
