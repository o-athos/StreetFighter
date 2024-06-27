#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int max_hp;
    int current_hp;
} HealthBar;

HealthBar* create_health_bar(int x, int y, int width, int height, int max_hp);
void update_health_bar(HealthBar *bar, int new_hp);
void draw_health_bar(ALLEGRO_DISPLAY *display, HealthBar *bar);
void destroy_health_bar(HealthBar *bar);

#endif