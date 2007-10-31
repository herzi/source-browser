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

#ifndef SB_REFERENCE_LABEL_H
#define SB_REFERENCE_LABEL_H

#include <gtk/gtk.h>
#include "sb-reference.h"

G_BEGIN_DECLS

typedef struct _SbReferenceLabel        SbReferenceLabel;
typedef struct _SbReferenceLabelPrivate SbReferenceLabelPrivate;
typedef struct _SbReferenceLabelClass   SbReferenceLabelClass;

#define SB_TYPE_REFERENCE_LABEL         (sb_reference_label_get_type ())
#define SB_REFERENCE_LABEL(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_REFERENCE_LABEL, SbReferenceLabel))

GtkWidget* sb_reference_label_new (SbReference* reference);

struct _SbReferenceLabel {
	GtkLabel                 base_instance;
	SbReferenceLabelPrivate* _private;
};

struct _SbReferenceLabelClass {
	GtkLabelClass            base_class;
};

G_END_DECLS

#endif /* !SB_REFERENCE_LABEL_H */
