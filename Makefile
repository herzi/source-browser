source_browser_SOURCES=\
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
	sb-marshallers.c \
	sb-marshallers.h \
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

all: source-browser test-async-io

source-browser: $(source_browser_SOURCES)
	gcc -g3 -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)

sb-marshallers.c: sb-marshallers.list
	@echo "Creating $@"
	@(echo "#include \"sb-marshallers.h\""; \
	 glib-genmarshal --body --prefix=sb_cclosure_marshal $< ) > $@

sb-marshallers.h: sb-marshallers.list
	@echo "Creating $@"
	@glib-genmarshal --header --prefix=sb_cclosure_marshal $< > $@

test-async-io: test-async-io.c
	gcc -g3 -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)

check: test-async-io
	@./test-async-io && echo "test-async-io passed" || (echo "FATAL: test-async-io failed"; exit 1)

clean:
	rm -rf source-browser test-async-io

.PHONY: check clean
