all: source-browser test-async-io

source-browser: sb-async-reader.c sb-async-reader.h sb-callback-data.c sb-callback-data.h sb-display.c sb-display.h sb-main.c sb-mem-pointer.c sb-mem-pointer.h sb-progress.c sb-progress.h sb-window.c sb-window.h
	gcc -g3 -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)

test-async-io: test-async-io.c
	gcc -g3 -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)

check: test-async-io
	@./test-async-io && echo "test-async-io passed" || (echo "FATAL: test-async-io failed"; exit 1)

clean:
	rm -rf source-browser test-async-io

.PHONY: check clean
