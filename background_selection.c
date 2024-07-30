#include "background_selection.h"

void choose_background(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, background *backgrounds[]) {

    int choice = 0;

    // Carregar imagens dos fundos
    ALLEGRO_BITMAP *bg1_image = al_load_bitmap("images/forest.png"); // Primeiro fundo
    ALLEGRO_BITMAP *bg2_image = al_load_bitmap("images/sunset.png"); // Segundo fundo

    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Desenhar o menu de escolha de fundo
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), 500, 100, ALLEGRO_ALIGN_CENTER, "CHOOSE THE MAP!");
        
        // Desenhar imagens dos fundos
        al_draw_scaled_bitmap(bg1_image, 0, 0, al_get_bitmap_width(bg1_image), al_get_bitmap_height(bg1_image),
                              100, 200, 300, 120, 0);

        al_draw_scaled_bitmap(bg2_image, 0, 0, al_get_bitmap_width(bg2_image), al_get_bitmap_height(bg2_image),
                              600, 200, 300, 120, 0);

        if (choice == 0) {
            al_draw_text(font, al_map_rgb(255, 255, 255), 250, 350, ALLEGRO_ALIGN_CENTER, "|");
        } else {
            al_draw_text(font, al_map_rgb(255, 255, 255), 750, 350, ALLEGRO_ALIGN_CENTER, "|");
        }

        al_flip_display();

        // Verificar eventos do teclado
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                choice = (choice + 1) % 2;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                choice = (choice + 1) % 2;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                break;
            }
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            exit(0);
        }
    }

    // Atribuir fundo escolhido ao array de fundos
    if (choice == 0) {
        backgrounds[0] = malloc(sizeof(background));
        backgrounds[0]->name = "Forest";
        backgrounds[0]->image_path = "images/forest.png";
    } else {
        backgrounds[0] = malloc(sizeof(background));
        backgrounds[0]->name = "Desert";
        backgrounds[0]->image_path = "images/sunset.png";
    }

    // Liberar imagens dos fundos
    al_destroy_bitmap(bg1_image);
    al_destroy_bitmap(bg2_image);
}
