bin_PROGRAMS=source-browser
check_PROGRAMS=test-async-io

BUILT_SOURCES=\
	sb-marshallers.c \
	sb-marshallers.h \
	$(NULL)
source_browser_SOURCES=\
	$(BUILT_SOURCES) \
	gobject-helpers.h \
	sb-annotations.c \
	sb-annotations.h \
	sb-async-reader.c \
	sb-async-reader.h \
	sb-comparable.c \
	sb-comparable.h \
	sb-contributor.c \
	sb-contributor.h \
	sb-display.c \
	sb-display.h \
	sb-main.c \
	sb-progress.c \
	sb-progress.h \
	sb-reference.c \
	sb-reference.h \
	sb-reference-label.c \
	sb-reference-label.h \
	sb-revision.c \
	sb-revision.h \
	sb-window.c \
	sb-window.h \
	$(NULL)
AM_CPPFLAGS=$(SB_CFLAGS)
LDADD=$(SB_LIBS)

sb-marshallers.c: sb-marshallers.list Makefile
	glib-genmarshal --prefix=sb_cclosure_marshal --body $< > $@

sb-marshallers.h: sb-marshallers.list Makefile
	glib-genmarshal --prefix=sb_cclosure_marshal --header $< > $@

