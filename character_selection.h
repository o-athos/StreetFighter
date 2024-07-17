#ifndef CHARACTER_SELECTION_H
#define CHARACTER_SELECTION_H


#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>


typedef struct character {
    char* name;
    char* spritesheet_path;
} character;

void choose_character (ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, character *characters[], int player);

#endif