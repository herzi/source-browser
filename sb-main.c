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

#include "sb-window.h"

#include <gio/gio.h>
#include <glib/gi18n.h>

int
main (int   argc,
      char**argv)
{
	gchar** files = NULL;
	GError* error = NULL;
	GOptionContext* context = g_option_context_new (_("[FILE]"));
	GOptionEntry entries[] = {
		{G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, &files, "", ""},
		{NULL}
	};

	g_option_context_set_help_enabled (context, TRUE);
	g_option_context_set_ignore_unknown_options (context, FALSE);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));
	g_option_context_add_main_entries (context, entries, NULL);
	if (!g_option_context_parse (context, &argc, &argv, &error)) {
		gchar* help = g_option_context_get_help (context, TRUE, NULL);
		g_printerr ("%s\nInvalid arguments%s%s\n",
			    help,
			    error ? ": " : "",
			    error ? error->message : "");
		g_option_context_free (context);
		g_clear_error (&error);
		g_free (help);
		return 1;
	}
	if (files && g_strv_length (files) > 1) {
		gchar* help = g_option_context_get_help (context, TRUE, NULL);
		g_printerr ("%s\nInvalid arguments: only one file supported\n",
			    help);
		g_option_context_free (context);
		g_free (help);
		return 1;
	}
	g_option_context_free (context);

	GtkWidget* window = sb_window_new ();
	gtk_widget_show (window);

	if (files) {
		GFile* file = g_file_new_for_commandline_arg (files[0]);
		gchar* path = g_file_get_path (file);
		sb_window_open (window, path);
		g_free (path);
		g_object_unref (file);
	}

	gtk_main ();
	return 0;
}

