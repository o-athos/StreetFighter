#ifndef BACKGROUND_SELECTION_H
#define BACKGROUND_SELECTION_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

typedef struct background {
    char* name;
    char* image_path;
} background;

void choose_background(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, background *backgrounds[]);

#endif