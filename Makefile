all: source-browser

source-browser: sb-main.c
	gcc -o $@ $^ $(shell pkg-config --cflags --libs gtk+-2.0)
