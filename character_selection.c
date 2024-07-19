#include "character_selection.h"

void choose_character(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, character *characters[], int player) {

    int choice = 0;

    // Carregar imagens dos personagens
    ALLEGRO_BITMAP *char1_image = al_load_bitmap("images/mike_choice.png");
    //ALLEGRO_BITMAP *char2_image = al_load_bitmap("path/to/char2_image.png");

    while (true){
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Desenhar o menu de escolha de personagem
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_textf(font, al_map_rgb(255, 255, 255), 500, 100, ALLEGRO_ALIGN_CENTER, "Jogador %d, escolha seu personagem", player);
        
        // Desenhar imagens dos personagens
        al_draw_scaled_bitmap(char1_image, 0, 0, al_get_bitmap_width(char1_image), al_get_bitmap_height(char1_image),
                              400, 200, 100, 120, 0);
        //al_draw_bitmap(char2_image, 400, 350, 0);

        //al_draw_text(font, al_map_rgb(255, 165, 0), 500, 300, ALLEGRO_ALIGN_CENTER, "Personagem 1");
        al_draw_text(font, al_map_rgb(255, 165, 0), 500, 350, ALLEGRO_ALIGN_CENTER, "Personagem 2");

        if (choice == 0) {
            al_draw_text(font, al_map_rgb(255, 255, 255), 350, 250, ALLEGRO_ALIGN_CENTER, ">");
        } else {
            al_draw_text(font, al_map_rgb(255, 255, 255), 350, 350, ALLEGRO_ALIGN_CENTER, ">");
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

    // Atribuir personagem escolhido ao array de personagens
    if (choice == 0) {
        characters[player - 1] = malloc(sizeof(character));
        characters[player - 1]->name = "Personagem 1";
        characters[player - 1]->spritesheet_path = "path/to/char1_spritesheet.png";
    } else {
        characters[player - 1] = malloc(sizeof(character));
        characters[player - 1]->name = "Personagem 2";
        characters[player - 1]->spritesheet_path = "path/to/char2_spritesheet.png";
    }

}