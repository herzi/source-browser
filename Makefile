all: source-browser test-async-io

source-browser: sb-main.c sb-window.c sb-window.h
	gcc -g3 -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)

test-async-io: test-async-io.c
	gcc -g3 -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)

check: test-async-io
	@./test-async-io && echo "test-async-io passed" || (echo "FATAL: test-async-io failed"; exit 1)

.PHONY: check
