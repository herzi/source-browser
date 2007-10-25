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

#ifndef SB_PROGRESS_H
#define SB_PROGRESS_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _SbProgress        SbProgress;
typedef struct _SbProgressPrivate SbProgressPrivate;
typedef struct _SbProgressClass   SbProgressClass;

#define SB_TYPE_PROGRESS         (sb_progress_get_type ())
#define SB_PROGRESS(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_PROGRESS, SbProgress))
#define SB_PROGRESS_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), SB_TYPE_PROGRESS, SbProgress))
#define SB_IS_PROGRESS(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), SB_TYPE_PROGRESS))
#define SB_IS_PROGRESS_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), SB_TYPE_PROGRESS))
#define SB_PROGRESS_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), SB_TYPE_PROGRESS, SbProgressClass))

GType      sb_progress_get_type   (void);
GtkWidget* sb_progress_new        (void);
gulong     sb_progress_get_status (SbProgress const* self);
void       sb_progress_set_status (SbProgress      * self,
				   gulong            status);
void       sb_progress_advance    (SbProgress      * self,
				   gulong            progress);
gulong     sb_progress_get_target (SbProgress const* self);
void       sb_progress_set_target (SbProgress      * self,
				   gulong            target);

struct _SbProgress {
	GtkProgressBar       base_instance;
	SbProgressPrivate*   _private;
};

struct _SbProgressClass {
	GtkProgressBarClass  base_class;
};

G_END_DECLS

#endif /* !SB_PROGRESS_H */
