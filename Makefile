all: source-browser

source-browser: sb-main.c
	gcc -g3 -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)
