CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5)
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5) -lm

all: streetFighter

streetFighter: streetFighter.c square.c joystick.c health_bar.c character_selection.c background_selection.c sprites.c bot.c
	$(CC) $(CFLAGS) streetFighter.c square.c joystick.c health_bar.c character_selection.c background_selection.c sprites.c bot.c -o streetFighter $(LDFLAGS)

clean:
	-rm -f streetFighter
	-rm -f *.swp