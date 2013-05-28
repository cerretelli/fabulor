CC=gcc -Wall
CFLAGS=-I. -lcrypto -lssl `pkg-config --cflags --libs gtk+-3.0 glib-2.0`
DEPS=window.h core.h
FILES=main.c window.c core.c
NAME=fabulor
DIR=./bin/

%.o: %.c $(DEPS)
	$(CC) -c -o $(DIR)$@ $< $(CFLAGS)

main: main.o 
	$(CC) -Wall -o $(DIR)$(NAME) $(FILES) $(CFLAGS)
debug: main.o
	$(CC) -Wall -g -o $(DIR)$(NAME) $(FILES) $(CFLAGS)
clean:
	rm -rf $(DIR)*
