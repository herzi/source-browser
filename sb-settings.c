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

#include "sb-settings.h"

#include <gconf/gconf-client.h>

static GConfClient*
get_client (void)
{
	static GConfClient* client = NULL;

	if (G_UNLIKELY (!client)) {
		client = gconf_client_get_default ();
	}

	return client;
}

gboolean
sb_settings_get_follow_copies (void)
{
	return gconf_client_get_bool (get_client (),
				      "/apps/source-browser/follow-copies",
				      NULL);
}

gboolean
sb_settings_get_follow_moves (void)
{
	return gconf_client_get_bool (get_client (),
				      "/apps/source-browser/follow-moves",
				      NULL);
}

gboolean
sb_settings_get_ignore_whitespaces (void)
{
	return gconf_client_get_bool (get_client (),
				      "/apps/source-browser/ignore-whitespaces",
				      NULL);
}

