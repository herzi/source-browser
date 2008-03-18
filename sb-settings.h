/* This file is part of source browser
 *
 * AUTHORS
 *     Sven Herzberg  <sven@imendio.com>
 *
 * Copyright (C) 2008  Sven Herzberg
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

#ifndef SB_SETTINGS_H
#define SB_SETTINGS_H

#include <glib.h>

G_BEGIN_DECLS

gboolean  sb_settings_get_follow_copies      (void);
gboolean  sb_settings_get_follow_moves       (void);
gboolean  sb_settings_get_ignore_whitespaces (void);

G_END_DECLS

#endif /* !SB_SETTINGS_H */
