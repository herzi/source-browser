all: source-browser

source-browser: sb-main.c
	gcc -o $@ $^
