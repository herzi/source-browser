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

#include "sb-display.h"

#include "sb-async-reader.h"
#include "sb-callback-data.h"

struct _SbDisplayPrivate {
	guint io_handler;

	/* the following are only valid during history loading */
	// FIXME: move them into an SbHistoryLoader
	SbAsyncReader* reader;
};

enum {
	LOAD_STARTED,
	LOAD_PROGRESS,
	// LOAD_DONE,
	// LOAD_CANCELLED,
	N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

G_DEFINE_TYPE (SbDisplay, sb_display, GTK_TYPE_TEXT_VIEW);

static void
sb_display_init (SbDisplay* self)
{
	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_DISPLAY,
						      SbDisplayPrivate);
}

static void
display_finalize (GObject* object)
{
	G_OBJECT_CLASS (sb_display_parent_class)->finalize (object);
}

static void
sb_display_class_init (SbDisplayClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);

	object_class->finalize = display_finalize;

	g_type_class_add_private (self_class, sizeof (SbDisplayPrivate));

	signals[LOAD_STARTED] = g_signal_new ("load-started",
					      SB_TYPE_DISPLAY,
					      0, 0,
					      NULL, NULL,
					      g_cclosure_marshal_VOID__VOID,
					      G_TYPE_NONE,
					      0);
	signals[LOAD_PROGRESS] = g_signal_new ("load-progress",
					       SB_TYPE_DISPLAY,
					       0, 0,
					       NULL, NULL,
					       g_cclosure_marshal_VOID__INT,
					       G_TYPE_NONE,
					       1, G_TYPE_INT);
}

GtkWidget*
sb_display_new (void)
{
	return g_object_new (SB_TYPE_DISPLAY, NULL);
}

static gboolean
io_watch_cb (GIOChannel  * channel,
	     GIOCondition  condition,
	     gpointer      data)
{
	GIOStatus state = G_IO_STATUS_NORMAL;
	static GString* string = NULL;
	static gchar* revision = NULL;
	SbDisplay* self = SB_DISPLAY (data);
	gunichar read = 0;

	if (G_UNLIKELY (!string)) {
		string = g_string_new ("");
	}

	state = g_io_channel_read_unichar (channel, &read, NULL);
	if (G_LIKELY (read != '\n')) {
		g_string_append_unichar (string, read);
	} else {
		if (!revision) {
			// "<40-byte hex sha1> <sourceline> <resultline> <num_lines>"
			gchar** words = g_strsplit (string->str, " ", -1);
			revision = g_strdup (words[0]);
			g_signal_emit_by_name (self,
					       "load-progress",
					       atoi (words[3]));
			g_strfreev (words);
		} else if (g_str_has_prefix (string->str, "filename ")) {
			g_print ("%s\n", revision);
			g_free (revision);
			revision = NULL;
#if 0
		} else {
			// FIXME: meta-information about the commit
			g_debug ("Got unknown line: %s",
				 string->str);
#endif
		}
		g_string_free (string, TRUE);
		string = NULL;
	}

	if (state == G_IO_STATUS_NORMAL) {
		return TRUE;
	} else {
		self->_private->io_handler = 0;
		return FALSE;
	}
}

static void
child_watch_cb (GPid pid,
		gint status_,
		gpointer data)
{
	SbDisplay * display = sb_callback_data_peek (data, "display"); // FIXME: call self
	GIOChannel* channel = sb_async_reader_get_channel (display->_private->reader);
	g_print ("pre-done.\n");
	if (display->_private->io_handler) {
		g_source_remove (display->_private->io_handler);
		while (io_watch_cb (channel, G_IO_IN, display))
			; // parse trailing lines
	}
	g_print ("done.\n");
	g_spawn_close_pid (pid);

	g_object_unref (display->_private->reader);
	display->_private->reader = NULL;
}

static inline void // FIXME: rename function
load_history (SbDisplay  * self,
	      gchar const* file_path)
{
	GIOChannel* out_chan;
	gchar* working_folder;
	gchar* argv[] = {
		"git-blame",
		"--incremental",
		"-M",
		"-C",
		NULL,
		NULL
	};
	GPid pid = 0;
	gint out_fd = 0;
	gpointer* channel_and_window;

	g_return_if_fail (!self->_private->reader); // protect against multiple execution

	working_folder = g_path_get_dirname (file_path);
	argv[2] = g_path_get_basename (file_path);
	gdk_spawn_on_screen_with_pipes (gtk_widget_get_screen (GTK_WIDGET (self)),
			     working_folder,
			     argv,
			     NULL,
			     G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
			     NULL,
			     NULL,
			     &pid,
			     NULL,
			     &out_fd,
			     NULL,
			     NULL); // FIXME: error, pipes

	self->_private->reader = sb_async_reader_new (out_fd);
	out_chan = sb_async_reader_get_channel (self->_private->reader);
	g_free (argv[2]);
	g_free (working_folder);

	self->_private->io_handler = g_io_add_watch (out_chan,
						     G_IO_IN,
						     io_watch_cb,
						     self);

	channel_and_window = sb_callback_data_new ("channel", out_chan,            g_io_channel_unref,
						   "display", g_object_ref (self), g_object_unref,
						   NULL);

	g_child_watch_add_full (G_PRIORITY_DEFAULT,
				pid,
				child_watch_cb,
				channel_and_window,
				sb_callback_data_free);
}

void
sb_display_load_path (SbDisplay  * self,
		      gchar const* path,
		      GError     **error)
{
	GtkTextBuffer* buffer;
	GMappedFile  * file;

	file = g_mapped_file_new (path, FALSE, error);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (self));

	gtk_text_buffer_set_text (buffer,
				  g_mapped_file_get_contents (file),
				  g_mapped_file_get_length (file));

	g_signal_emit (self, signals[LOAD_STARTED], 0);

	g_mapped_file_free (file);

	load_history (self,
		      path);

	// FIXME: disable loading of new files until the history is loaded
	// FIXME: make history loading cancellable
}

