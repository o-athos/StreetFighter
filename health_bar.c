#include "health_bar.h"
#include <stdlib.h>

HealthBar* create_health_bar(int x, int y, int width, int height, int max_hp) {
    HealthBar *bar = (HealthBar*)malloc(sizeof(HealthBar));
    if (!bar) return NULL;

    bar->x = x;
    bar->y = y;
    bar->width = width;
    bar->height = height;
    bar->max_hp = max_hp;
    bar->current_hp = max_hp;

    return bar;
}

void update_health_bar(HealthBar *bar, int new_hp) {
    if (new_hp < 0) {
        bar->current_hp = 0;
    } else if (new_hp > bar->max_hp) {
        bar->current_hp = bar->max_hp;
    } else {
        bar->current_hp = new_hp;
    }
}

void draw_health_bar(ALLEGRO_DISPLAY *display, HealthBar *bar) {
    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR green = al_map_rgb(0, 255, 0);

    int bar_width = bar->width * bar->current_hp / bar->max_hp;

    al_draw_filled_rectangle(bar->x, bar->y, bar->x + bar->width, bar->y + bar->height, red);
    al_draw_filled_rectangle(bar->x, bar->y, bar->x + bar_width, bar->y + bar->height, green);
}

void destroy_health_bar(HealthBar *bar) {
    free(bar);
}
