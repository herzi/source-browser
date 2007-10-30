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

#ifndef SB_DISPLAY_H
#define SB_DISPLAY_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _SbDisplay        SbDisplay;
typedef struct _SbDisplayPrivate SbDisplayPrivate;
typedef struct _SbDisplayClass   SbDisplayClass;

#define SB_TYPE_DISPLAY         (sb_display_get_type ())
#define SB_DISPLAY(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_DISPLAY, SbDisplay))
#define SB_DISPLAY_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), SB_TYPE_DISPLAY, SbDisplayClass))
#define SB_IS_DISPLAY(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), SB_TYPE_DISPLAY))
#define SB_IS_DISPLAY_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), SB_TYPE_DISPLAY))
#define SB_DISPLAY_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS ((i), SB_TYPE_DISPLAY, SbDisplayClass))

GType      sb_display_get_type    (void);
GtkWidget* sb_display_new         (void);
gint       sb_display_get_n_lines (SbDisplay const* self);
void       sb_display_load_path   (SbDisplay      * self,
				   gchar const    * path,
				   GError         **error);

struct _SbDisplay {
	GtkTextView       base_instance;
	SbDisplayPrivate* _private;
};

struct _SbDisplayClass {
	GtkTextViewClass  base_class;
};

G_END_DECLS

#endif /* !SB_DISPLAY_H */
