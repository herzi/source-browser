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

#include "sb-annotations.h"
#include "sb-async-reader.h"
#include "sb-callback-data.h"
#include "sb-comparable.h"
#include "sb-marshallers.h"
#include "sb-reference.h"

struct _SbDisplayPrivate {
	SbAnnotations* annotations;
	GtkTextView  * text_view;
	GList        * references;

	/* the following are only valid during history loading */
	// FIXME: move them into an SbHistoryLoader
	SbAsyncReader* reader;
	GHashTable   * revisions;
	SbRevision   * revision; // FIXME: drop this one
	SbReference  * reference;
};

enum {
	LOAD_STARTED,
	LOAD_PROGRESS,
	// FIXME: LOAD_DONE,
	// FIXME: LOAD_CANCELLED,
	N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

G_DEFINE_TYPE (SbDisplay, sb_display, GTK_TYPE_HBOX);

static void
sb_display_init (SbDisplay* self)
{
	GtkWidget* widget;

	self->_private = G_TYPE_INSTANCE_GET_PRIVATE (self,
						      SB_TYPE_DISPLAY,
						      SbDisplayPrivate);

	widget = sb_annotations_new ();
	gtk_widget_show (widget);
	gtk_box_pack_start (GTK_BOX (self),
			    widget,
			    FALSE,
			    FALSE,
			    0);
	self->_private->annotations = SB_ANNOTATIONS (widget);

	widget = gtk_text_view_new ();
	gtk_widget_show (widget);
	gtk_box_pack_start_defaults (GTK_BOX (self),
				     widget);
	self->_private->text_view = GTK_TEXT_VIEW (widget);

	self->_private->revisions = g_hash_table_new_full ((GHashFunc)sb_comparable_hash,
							   (GEqualFunc)sb_comparable_equals,
							   NULL,
							   g_object_unref);
}

static void
display_finalize (GObject* object)
{
	SbDisplay* self = SB_DISPLAY (object);

	g_hash_table_destroy (self->_private->revisions);

	G_OBJECT_CLASS (sb_display_parent_class)->finalize (object);
}

static void
display_set_scroll_adjustments (SbDisplay    * self,
				GtkAdjustment* horizontal,
				GtkAdjustment* vertical)
{
	gtk_widget_set_scroll_adjustments (GTK_WIDGET (self->_private->annotations),
					   horizontal,
					   vertical);
	gtk_widget_set_scroll_adjustments (GTK_WIDGET (self->_private->text_view),
					   horizontal,
					   vertical);
}

static void
sb_display_class_init (SbDisplayClass* self_class)
{
	GObjectClass* object_class = G_OBJECT_CLASS (self_class);
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (self_class);

	object_class->finalize             = display_finalize;

	self_class->set_scroll_adjustments = display_set_scroll_adjustments;

	g_type_class_add_private (self_class, sizeof (SbDisplayPrivate));

	widget_class->set_scroll_adjustments_signal =
				g_signal_new ("set-scroll-adjustments",
					      SB_TYPE_DISPLAY,
					      G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET (SbDisplayClass, set_scroll_adjustments),
					      NULL, NULL,
					      sb_cclosure_marshal_VOID__BOXED_BOXED,
					      G_TYPE_NONE, 2,
					      GTK_TYPE_ADJUSTMENT,
					      GTK_TYPE_ADJUSTMENT);

	signals[LOAD_STARTED] = g_signal_new ("load-started",
					      SB_TYPE_DISPLAY,
					      0, 0,
					      NULL, NULL,
					      g_cclosure_marshal_VOID__VOID,
					      G_TYPE_NONE, 0);
	signals[LOAD_PROGRESS] = g_signal_new ("load-progress",
					       SB_TYPE_DISPLAY,
					       0, 0,
					       NULL, NULL,
					       g_cclosure_marshal_VOID__INT,
					       G_TYPE_NONE, 1,
					       G_TYPE_INT);
}

GtkWidget*
sb_display_new (void)
{
	return g_object_new (SB_TYPE_DISPLAY, NULL);
}

gint
sb_display_get_n_lines (SbDisplay const* self)
{
	g_return_val_if_fail (SB_IS_DISPLAY (self), 0);

	return gtk_text_buffer_get_line_count (gtk_text_view_get_buffer (self->_private->text_view));
}

static gint
sort_refs_by_target_line (gconstpointer a,
			  gconstpointer b)
{
	return sb_reference_get_current_start (a) - sb_reference_get_current_start (b);
}

static void
display_parse_line (SbAsyncReader* reader,
		    gchar const  * line,
		    SbDisplay    * self)
{
	g_return_if_fail (SB_IS_DISPLAY (self));

	// FIXME: make sure we have a new hash table each time
	if (G_UNLIKELY (!self->_private->revision)) {
		/* from git-annotate (1)
		 * "<40-byte hex sha1> <sourceline> <resultline> <num_lines>"
		 */
		gchar     **words = g_strsplit (line, " ", -1);
		SbRevision* revision = sb_revision_new (words[0]);
		gint        n_lines  = atoi (words[3]);

		self->_private->revision = g_hash_table_lookup (self->_private->revisions,
								revision);

		if (!self->_private->revision) {
			self->_private->revision = g_object_ref (revision);
			// FIXME: create an API for SbHashedVector
			g_hash_table_insert (self->_private->revisions,
					     self->_private->revision,
					     self->_private->revision);
		}

		self->_private->reference = sb_reference_new (self->_private->revision,
							      atoi (words[2]));

		g_signal_emit (self,
			       signals[LOAD_PROGRESS],
			       0,
			       n_lines);

		g_object_unref (revision);
		g_strfreev (words);
	} else if (g_str_has_prefix (line, "filename ")) {
		g_print ("%s\n", sb_revision_get_name (self->_private->revision));
		self->_private->revision = NULL;
		// FIXME: use a GSequence for the revisions (maybe that would make the API nicer too)
		self->_private->references = g_list_insert_sorted (self->_private->references,
								   g_object_ref (self->_private->reference),
								   sort_refs_by_target_line);
		g_object_unref (self->_private->reference);
		self->_private->reference = NULL;
#if 0
	} else {
		// FIXME: meta-information about the commit
		g_debug ("Got unknown line: %s",
			 line);
#endif
	}
}

static void
debug_reference (SbReference const* self)
{
	g_print ("%3.d: %s\n",
		 sb_reference_get_current_start (self),
		 sb_revision_get_name (sb_reference_get_revision (self)));
}

static void
child_watch_cb (GPid pid,
		gint status_,
		gpointer data)
{
	SbDisplay * display = SB_DISPLAY (data); // FIXME: call self

	g_print ("pre-done.\n");
	sb_reader_flush (display->_private->reader);
	g_print ("done.\n");
	g_spawn_close_pid (pid);

	g_list_foreach (display->_private->references, (GFunc)debug_reference, NULL);

	g_object_unref (display->_private->reader);
	display->_private->reader = NULL;
}

static inline void // FIXME: rename function
load_history (SbDisplay  * self,
	      gchar const* file_path)
{
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

	g_return_if_fail (!self->_private->reader); // protect against multiple execution

	g_list_foreach (self->_private->references, (GFunc)g_object_unref, NULL);
	g_list_free    (self->_private->references);

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
	g_free (argv[2]);
	g_free (working_folder);

	g_signal_connect (self->_private->reader, "read-line",
			  G_CALLBACK (display_parse_line), self);

	g_child_watch_add_full (G_PRIORITY_DEFAULT,
				pid,
				child_watch_cb,
				self,
				NULL);
}

void
sb_display_load_path (SbDisplay  * self,
		      gchar const* path,
		      GError     **error)
{
	GtkTextBuffer* buffer;
	GMappedFile  * file;

	file = g_mapped_file_new (path, FALSE, error);
	buffer = gtk_text_view_get_buffer (self->_private->text_view);

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

