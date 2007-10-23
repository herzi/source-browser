/* This file is part of source browser
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@gnome-de.org>
 *
 * Copyright (C) 2007  Sven Herzberg
 *
 * This work is provided "as is"; redistribution and modification
 * in whole or in part, in any medium, physical or electronic is
 * permitted without restriction.
 *
 * This work is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In no event shall the authors or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 */

#include <glib.h>

static gboolean
idle_cb (gpointer unused)
{
	return FALSE;
}

int
main (int   argc,
      char**argv)
{
	GMainLoop* loop;
	guint      idle_handler;

	loop = g_main_loop_new (NULL, FALSE);
	idle_handler = g_idle_add_full (G_MAXINT,
					idle_cb,
					NULL,
					NULL);

	g_source_remove   (idle_handler);
	g_main_loop_unref (loop);

	return 0;
}

