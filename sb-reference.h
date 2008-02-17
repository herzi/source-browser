/* This file is part of source browser
 *
 * AUTHORS
 *     Sven Herzberg
 *
 * Copyright (C) 2007,2008  Sven Herzberg
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

#ifndef SB_REFERENCE_H
#define SB_REFERENCE_H

#include "sb-revision.h"

G_BEGIN_DECLS

typedef struct _SbReference        SbReference;
typedef struct _SbReferencePrivate SbReferencePrivate;
typedef struct _SbReferenceClass   SbReferenceClass;

#define SB_TYPE_REFERENCE         (sb_reference_get_type ())
#define SB_REFERENCE(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_REFERENCE, SbReference))
#define SB_IS_REFERENCE(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), SB_TYPE_REFERENCE))

GType        sb_reference_get_type	    (void);
SbReference* sb_reference_new		    (SbRevision       * revision,
					     guint              current_start,
					     guint              current_end);
guint        sb_reference_get_current_start (SbReference const* self);
guint        sb_reference_get_current_end   (SbReference const* self);
gchar const* sb_reference_get_filename      (SbReference const* self);
SbRevision*  sb_reference_get_revision      (SbReference const* self);
void         sb_reference_set_filename      (SbReference      * self,
					     gchar const      * filename);

struct _SbReference {
	GObject             base_instance;
	SbReferencePrivate* _private;
};

struct _SbReferenceClass {
	GObjectClass base_class;
};

G_END_DECLS

#endif /* !SB_REFERENCE_H */
