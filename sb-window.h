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

#ifndef SB_WINDOW_H
#define SB_WINDOW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _SbWindow        SbWindow;
typedef struct _SbWindowPrivate SbWindowPrivate;
typedef struct _SbWindowClass   SbWindowClass;

#define SB_TYPE_WINDOW         (sb_window_get_type ())
#define SB_WINDOW(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), SB_TYPE_WINDOW, SbWindow))

GType      sb_window_get_type    (void);
GtkWidget* sb_window_new         (void);
GtkWidget* sb_window_get_display (GtkWidget  * self);
GtkWidget* sb_window_get_status  (GtkWidget  * self);
GtkWidget* sb_window_get_swin    (GtkWidget  * self);
GtkWidget* sb_window_get_vbox    (GtkWidget  * self);
void       sb_window_open        (GtkWidget  * self,
				  gchar const* path);

struct _SbWindow {
	GtkWindow        base_instance;
	SbWindowPrivate* _private;
};

struct _SbWindowClass {
	GtkWindowClass   base_class;
};

G_END_DECLS

#endif /* !SB_WINDOW_H */
