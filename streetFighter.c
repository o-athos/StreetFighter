/* 
Compilação: 
gcc streetFighter.c square.c joystick.c  health_bar.c character_selection.c background_selection.c sprites.c bot.c -lm -o streetFighter $(pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5)
*/

#include <stdio.h>
#include <stdlib.h>

//bibliotecas allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

//bibliotecas pessoais
#include "square.h"
#include "joystick.h"
#include "character_selection.h"
#include "background_selection.h"
#include "sprites.h"
#include "bot.h"

#define X_SCREEN 1000
#define Y_SCREEN 500
#define FLOOR 420
#define HIGH_JUMP 280
#define JUMP_SPEED 10
#define GRAVITY 5


void update_position(square *player_1, square *player_2){																																				
	
	if (player_1->control->left && !player_1->is_crouching && !player_1->control->parry){	
		if ((player_1->is_punching && (player_1->is_jump || player_1->is_faling)) || (player_1->is_kicking && (player_1->is_jump || player_1->is_faling))){																																									
			square_move(player_1, 1, 0, X_SCREEN, FLOOR);																																																												
			player_1->face = 0;
		}
		if (!player_1->is_punching && !player_1->is_kicking){
			square_move(player_1, 1, 0, X_SCREEN, FLOOR);																																																												
			player_1->face = 0;
		}
	}
	if (player_1->control->right && !player_1->is_crouching && !player_1->control->parry){																																									
		if ((player_1->is_punching && (player_1->is_jump || player_1->is_faling)) || (player_1->is_kicking && (player_1->is_jump || player_1->is_faling))){																																									
			square_move(player_1, 1, 1, X_SCREEN, FLOOR);																																																												
			player_1->face = 1;
		}
		if (!player_1->is_punching && !player_1->is_kicking){
			square_move(player_1, 1, 1, X_SCREEN, FLOOR);																																																												
			player_1->face = 1;
		}
	}
	if (player_1->control->up) {																																										
		square_move(player_1, 1, 2, X_SCREEN, FLOOR);																																																															
	}
	if (player_1->control->down && !player_1->is_jump && !player_1->is_faling){																																										
		square_move(player_1, 1, 3, X_SCREEN, FLOOR);																																																												
	}

	if (player_2->control->left && !player_2->is_crouching && !player_2->control->parry){	
		if ((player_2->is_punching && (player_2->is_jump || player_2->is_faling)) || (player_2->is_kicking && (player_2->is_jump || player_2->is_faling))){																																									
			square_move(player_2, 1, 0, X_SCREEN, FLOOR);																																																												
			player_2->face = 0;
		}
		if (!player_2->is_punching && !player_2->is_kicking){
			square_move(player_2, 1, 0, X_SCREEN, FLOOR);																																																												
			player_2->face = 0;
		}
	}
	
	if (player_2->control->right && !player_2->is_crouching && !player_2->control->parry){																																									
		if ((player_2->is_punching && (player_2->is_jump || player_2->is_faling)) || (player_2->is_kicking && (player_2->is_jump || player_2->is_faling))){																																									
			square_move(player_2, 1, 1, X_SCREEN, FLOOR);																																																												
			player_2->face = 1;
		}
		if (!player_2->is_punching && !player_2->is_kicking){
			square_move(player_2, 1, 1, X_SCREEN, FLOOR);																																																												
			player_2->face = 1;
		}
	}
	
	if (player_2->control->up){																																											
		square_move(player_2, 1, 2, X_SCREEN, FLOOR);																													
	}
	if (player_2->control->down && !player_2->is_jump && !player_2->is_faling){																																										
		square_move(player_2, 1, 3, X_SCREEN, FLOOR);																																																													
	}


	/* PULO DO PLAYER 1 */
	if (player_1->control->jump ){
		square_jump(player_1, FLOOR);
	}
	
	if (player_1->is_jump){
		player_1->y -= JUMP_SPEED;
		if (player_1->y <= HIGH_JUMP){
			player_1->is_faling = 1;
			player_1->is_jump = 0;
		}
	}

	if (player_1->is_faling){
		player_1->y += GRAVITY;
		if (player_1->y >= FLOOR - player_1->y_side/2){
			player_1->y = FLOOR - player_1->y_side/2;
			player_1->is_faling = 0;
		}
	}



	/* PULO DO PLAYER 2 */
	if (player_2->control->jump){
		square_jump(player_2, FLOOR);
	}
	
	if (player_2->is_jump){
		player_2->y -= JUMP_SPEED;
		if (player_2->y <= HIGH_JUMP){
			player_2->is_faling = 1;
			player_2->is_jump = 0;
		}
	}

	if (player_2->is_faling){
		player_2->y += GRAVITY;
		if (player_2->y >= FLOOR - player_2->y_side/2){
			player_2->y = FLOOR - player_2->y_side/2;
			player_2->is_faling = 0;
		}
	}


	/* SE ABAIXA */
	if (player_1->control->crouch && !player_1->is_faling){
		square_crouch(player_1);
	} else {
		square_stand(player_1);
	}
    if (player_2->control->crouch && !player_2->is_faling) {
        square_crouch(player_2);
    } else {
        square_stand(player_2);
    }


	/* SOCO  */
	if (player_1->control->punch && !player_1->is_kicking ){
		if (!player_1->punch_timer){
			square_punch(player_1, player_2);
			player_1->is_punching = 1;
			player_1->punch_timer = PUNCH_COOLDOWN;
		}
	}
	if (player_2->control->punch && !player_2->is_kicking){
		if (!player_2->punch_timer){
			square_punch(player_2, player_1);
			player_2->is_punching = 1;
			player_2->punch_timer = PUNCH_COOLDOWN;
		}
	}

	/* CHUTE */
	if (player_1->control->kick && !player_1->is_punching){
		if (!player_1->kick_timer){
			square_kick(player_1, player_2);
			player_1->is_kicking = 1;
			player_1->kick_timer = KICK_COOLDOWN;
		}
	}
	if (player_2->control->kick && !player_2->is_punching){
		if (!player_2->kick_timer){
			square_kick(player_2, player_1);
			player_2->is_kicking = 1;
			player_2->kick_timer = KICK_COOLDOWN;
		}
	}
}



int main() {

    al_init();

    al_init_primitives_addon();

    al_install_keyboard();

    al_init_font_addon();
    al_init_ttf_addon();

    al_init_image_addon();

	srand(time(NULL));
        


    // Criar temporizador, fila de eventos e display
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_FONT *font = al_load_ttf_font("fonts/Pixellari.ttf", 30, 0);
	ALLEGRO_FONT *font_name = al_load_ttf_font("fonts/American_Captain.ttf", 20, 0);


    // Registrar fontes de eventos
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // Iniciar temporizador
    al_start_timer(timer);


	ALLEGRO_BITMAP *title_game = al_load_bitmap("images/street_fighter.png");
	ALLEGRO_BITMAP *pause_image = al_load_bitmap("images/pause.png");

    int desired_width = 500;
    int desired_height = 200;

    // Variável para controlar o estado do menu
    bool menu = true;

   // Loop principal do jogo
    while (menu) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Desenhar o menu na tela
        al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpar a tela

		int image_width = al_get_bitmap_width(title_game);
        int image_height = al_get_bitmap_height(title_game);
		al_draw_scaled_bitmap(
			title_game,
			0, 0, // Fonte X, Y
			image_width, image_height, // Largura e altura da fonte
			(1000 - desired_width) / 2, (600 - desired_height) / 3, // Destino X, Y
			desired_width, desired_height, // Largura e altura desejadas
			0 // Nenhuma flag
		);

        al_draw_text(font, al_map_rgb(255, 255, 255), 500, 400, ALLEGRO_ALIGN_CENTER, "Pressione Enter para jogar"); // Mensagem para pressionar Enter

        al_flip_display(); // Atualizar o display

        // Verificar eventos do teclado
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                menu = false; // Sair do menu se Enter for pressionado
            }
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return 0; // Fechar o jogo se o usuário fechar a janela
        }
    }

	bool game_mode_menu = 1;
	int bot, choice;

	while (game_mode_menu){

		ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Desenhar o menu de escolha de personagem
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 165, 0), 400, 100, 0, "SINGLEPLAYER");
		al_draw_text(font, al_map_rgb(255, 165, 0), 400, 300, 0, "MULTIPLAYER");	


        if (choice == 0) {
            al_draw_text(font, al_map_rgb(255, 255, 255), 300, 100, ALLEGRO_ALIGN_CENTER, "->");
        } else {
            al_draw_text(font, al_map_rgb(255, 255, 255), 300, 300, ALLEGRO_ALIGN_CENTER, "->");
        }

		al_flip_display();

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
	
	if (choice == 0)
		bot = 1;
	else
		bot = 0;


	/* -------------------------INICIO DO JOGO COM ROUNDS --------------------------------*/


	int play_again = 1;
	while (play_again){
	
		character *characters[2];
    	choose_character(queue, font, characters, 1);
    	choose_character(queue, font, characters, 2);

		Character* character1;
		Character* character2;

		if (strcmp(characters[0]->name, "Ken") == 0)
			character1 = load_character(characters[0]->spritesheet_path, 5, 4, 1, 5, 3, 7, 1, 1, 3, 3, 3, 3, 2, 100, 120);
		else if (strcmp(characters[0]->name, "Ryu") == 0)
			character1 = load_character(characters[0]->spritesheet_path, 5, 4, 1, 3, 4, 7, 1, 1, 4, 3, 3, 3, 2, 100, 120);

		if (strcmp(characters[1]->name, "Ken") == 0)
			character2 = load_character(characters[1]->spritesheet_path, 5, 4, 1, 5, 3, 7, 1, 1, 3, 3, 3, 3, 2, 100, 120);
		else if (strcmp(characters[1]->name, "Ryu") == 0)
			character2 = load_character(characters[1]->spritesheet_path, 5, 4, 1, 3, 4, 7, 1, 1, 4, 3, 3, 3, 2, 100, 120);

		char path[256];
		snprintf(path, sizeof(path), "%s/face.png", characters[0]->spritesheet_path);
		ALLEGRO_BITMAP* char1_vs = al_load_bitmap(path);

		snprintf(path, sizeof(path), "%s/face.png", characters[1]->spritesheet_path);
		ALLEGRO_BITMAP* char2_vs = al_load_bitmap(path);

		ALLEGRO_BITMAP* vs = al_load_bitmap("images/vs.png");
		
		background* background_choose[1];
		choose_background(queue, font, background_choose);
		ALLEGRO_BITMAP *background = al_load_bitmap(background_choose[0]->image_path);

		// TELA DE 'VS'
		while (true){
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_scaled_bitmap(char1_vs, 
						0, 0, al_get_bitmap_width(char1_vs), al_get_bitmap_height(char1_vs), 
						X_SCREEN / 6 , Y_SCREEN / 3, 150, 150, 
						0);
			al_draw_scaled_bitmap(vs, 
						0, 0, al_get_bitmap_width(vs), al_get_bitmap_height(vs), 
						X_SCREEN / 2 - 60, Y_SCREEN / 3, 150, 150, 
						0);
			al_draw_scaled_bitmap(char2_vs, 
						0, 0, al_get_bitmap_width(char2_vs), al_get_bitmap_height(char2_vs), 
						X_SCREEN - 280, Y_SCREEN / 3, 150, 150, 
						ALLEGRO_FLIP_HORIZONTAL);
			

			al_flip_display();

			al_rest(3.0);
			break;
		}


		int p1_score = 0, p2_score = 0;
		int rounds = 0;
		while (p1_score - p2_score != 2 && p2_score - p1_score != 2 && rounds < 3){

			square* player_1 = square_create(50, 100, 1, 50, FLOOR-50, X_SCREEN, Y_SCREEN, bot);				//Cria o quadrado do primeiro jogador
			if (!player_1) return 1;																//Verificação de erro na criação do quadrado do primeiro jogador
			square* player_2 = square_create(50, 100, 0, X_SCREEN-50, FLOOR-50, X_SCREEN, Y_SCREEN, bot);		//Cria o quadrado do segundo jogador
			if (!player_2) return 2;

			player_1->health_bar = create_health_bar(10, 20, 420, 25, player_1->hp);
			player_2->health_bar = create_health_bar(X_SCREEN - 430, 20, 420, 25, player_2->hp);

			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_scaled_bitmap(background, 
				0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 
				0, 0, X_SCREEN, Y_SCREEN, 
				0);

			al_draw_filled_rectangle(player_1->x-player_1->x_side/2, player_1->y-player_1->y_side/2, player_1->x+player_1->x_side/2, player_1->y+player_1->y_side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
			al_draw_filled_rectangle(player_2->x-player_2->x_side/2, player_2->y-player_2->y_side/2, player_2->x+player_2->x_side/2, player_2->y+player_2->y_side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela

			draw_health_bar(disp, player_1->health_bar);
			draw_health_bar(disp, player_2->health_bar);

			al_draw_text(font_name, al_map_rgb(255, 140, 0), 20, 25, 0, characters[0]->name);
			al_draw_text(font_name, al_map_rgb(255, 140, 0), X_SCREEN - 55, 25, 0, characters[1]->name);

			al_draw_scaled_bitmap(vs, 
						0, 0, al_get_bitmap_width(vs), al_get_bitmap_height(vs), 
						X_SCREEN / 2 -20 , 10, 40, 40, 
						0);

			draw_animation(character1, player_1->x-2*player_1->x_side, player_1->y-player_1->y_side/2, player_1->face, 0);
			draw_animation(character2, player_2->x-2*player_2->x_side, player_2->y-player_2->y_side/2, player_2->face, 0);

			char round_message[20];

			sprintf(round_message, "Round %d", rounds + 1);
			al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, round_message);
			al_flip_display();
			al_rest(1.0);

			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_scaled_bitmap(background, 
				0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 
				0, 0, X_SCREEN, Y_SCREEN, 
				0);

			al_draw_filled_rectangle(player_1->x-player_1->x_side/2, player_1->y-player_1->y_side/2, player_1->x+player_1->x_side/2, player_1->y+player_1->y_side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
			al_draw_filled_rectangle(player_2->x-player_2->x_side/2, player_2->y-player_2->y_side/2, player_2->x+player_2->x_side/2, player_2->y+player_2->y_side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela

			draw_health_bar(disp, player_1->health_bar);
			draw_health_bar(disp, player_2->health_bar);
			
			al_draw_text(font_name, al_map_rgb(255, 140, 0), 20, 25, 0, characters[0]->name);
			al_draw_text(font_name, al_map_rgb(255, 140, 0), X_SCREEN - 55, 25, 0, characters[1]->name);

			al_draw_scaled_bitmap(vs, 
						0, 0, al_get_bitmap_width(vs), al_get_bitmap_height(vs), 
						X_SCREEN / 2 - 20 , 10, 40, 40, 
						0);

			draw_animation(character1, player_1->x-2*player_1->x_side, player_1->y-player_1->y_side/2, player_1->face, 0);
			draw_animation(character2, player_2->x-2*player_2->x_side, player_2->y-player_2->y_side/2, player_2->face, 0);

			al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Fight!");
			al_flip_display();
			al_rest(1.0);	

			al_flush_event_queue(queue);

			// Tempo inicial
			double last_time = al_get_time();
			double current_time;
			float delta_time;

			unsigned char p1_isDead = 0, p2_isDead = 0, round_over = 0, pause = 0;
			ALLEGRO_EVENT event;
			if (event.type == 42) return 1;
			while(1){																																															//Laço principal do programa
				al_wait_for_event(queue, &event);																																								//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
				
				if (pause) {
					al_clear_to_color(al_map_rgb(0, 0, 0));

					int image_width = al_get_bitmap_width(pause_image);
					int image_height = al_get_bitmap_height(pause_image);
					al_draw_scaled_bitmap(
						pause_image,
						0, 0, // Fonte X, Y
						image_width, image_height, // Largura e altura da fonte
						0, 0, // Destino X, Y
						X_SCREEN, Y_SCREEN, // Largura e altura desejadas
						0 // Nenhuma flag
					);

					al_flip_display();
					
					// Esperar até que o botão de pausa seja pressionado novamente
					while (1) {
						al_wait_for_event(queue, &event);
						if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
							if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
								pause = 0;  // Despausar o jogo
								break;
							}
						}
					}
					continue;  // Voltar para o início do loop principal
				}
				
				// Calcular delta_time
				current_time = al_get_time();
				delta_time = (float)(current_time - last_time);
				last_time = current_time;

				if (round_over) break;

				if (p1_isDead || p2_isDead ){
					al_clear_to_color(al_map_rgb(0, 0, 0));																																							
					if (p1_isDead && p2_isDead) al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 20, Y_SCREEN/2 - 15, 0, "EMPATE!");																					
					else if (p2_isDead){
						p1_score++;
						printf("p1:%d p2:%d\n", p1_score, p2_score);
						al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 120, Y_SCREEN/2-15, 0, "JOGADOR 1 GANHOU!");																				
					}
					else if (p1_isDead){
						p2_score++;
						al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 - 120, Y_SCREEN/2-15, 0, "JOGADOR 2 GANHOU!");
					}																																								
					al_flip_display();																																												

					al_rest(3.0);

					al_flush_event_queue(queue);

					round_over = 1;
				}
				else {
					if (event.type == 30){

						update_position(player_1, player_2);	

						update_character_status(character1, player_1);
						if (!player_2->is_bot) {
							update_character_status(character2, player_2);
						} else {
							// Atualize o bot joystick se player_2 for um bot
							update_bot_joystick(player_2, player_1, character2);
							update_character_status(character2, player_2);
						}
						
						if (player_1->hp == 0)
							p1_isDead = 1;
						if (player_2->hp == 0)
							p2_isDead = 1;
																																										
						al_clear_to_color(al_map_rgb(0, 0, 0));	

						al_draw_scaled_bitmap(background, 
							0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 
							0, 0, X_SCREEN, Y_SCREEN, 
							0);
																																																																								//Substitui tudo que estava desenhado na tela por um fundo preto
						al_draw_filled_rectangle(player_1->x-player_1->x_side/2, player_1->y-player_1->y_side/2, player_1->x+player_1->x_side/2, player_1->y+player_1->y_side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
						al_draw_filled_rectangle(player_2->x-player_2->x_side/2, player_2->y-player_2->y_side/2, player_2->x+player_2->x_side/2, player_2->y+player_2->y_side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela
						
						draw_health_bar(disp, player_1->health_bar);
						draw_health_bar(disp, player_2->health_bar);

						al_draw_text(font_name, al_map_rgb(255, 140, 0), 20, 25, 0, characters[0]->name);
						al_draw_text(font_name, al_map_rgb(255, 140, 0), X_SCREEN - 55, 25, 0, characters[1]->name);

						al_draw_scaled_bitmap(vs, 
							0, 0, al_get_bitmap_width(vs), al_get_bitmap_height(vs), 
							X_SCREEN / 2 - 20, 10, 40, 40, 
							0);

						draw_animation(character1, player_1->x-2*player_1->x_side, player_1->y-player_1->y_side/2, player_1->face, delta_time);
						draw_animation(character2, player_2->x-2*player_2->x_side, player_2->y-player_2->y_side/2, player_2->face, delta_time);

						if (p1_score == 1)
							//al_draw_filled_circle(15, 25, 5, al_map_rgb(255, 0, 0));
							al_draw_scaled_bitmap(char1_vs, 
									0, 0, al_get_bitmap_width(char1_vs), al_get_bitmap_height(char1_vs), 
									15 , 50, 20, 20, 
									0);
						if (p2_score == 1){
							al_draw_scaled_bitmap(char2_vs, 
									0, 0, al_get_bitmap_width(char2_vs), al_get_bitmap_height(char2_vs), 
									X_SCREEN - 35 , 50, 20, 20, 
									0);
						}


						if (player_1->punch_timer > 0) 
							player_1->punch_timer--;
						else
							player_1->is_punching = 0;
						
						if (player_2->punch_timer > 0)
							player_2->punch_timer--;
						else
							player_2->is_punching = 0;


						if (player_1->kick_timer > 0)
							 player_1->kick_timer--;
						else 
							player_1->is_kicking = 0;
						if (player_2->kick_timer > 0) 
							player_2->kick_timer--;
						else
							player_2->is_kicking = 0;	

						al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
					}
					else if ((event.type == 10) || (event.type == 12)){	

						if (event.type == 10){
							if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
								pause = !pause;
							}
						}

						if (event.keyboard.keycode == ALLEGRO_KEY_A) 			joystick_left(player_1->control);																															
						else if (event.keyboard.keycode == ALLEGRO_KEY_D) 		joystick_right(player_1->control);																																			
						else if (event.keyboard.keycode == ALLEGRO_KEY_S) 		joystick_crouch(player_1->control);		
						else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) 	joystick_jump(player_1->control);
						else if (event.keyboard.keycode == ALLEGRO_KEY_E) 		joystick_parry(player_1->control);						
						else if (event.keyboard.keycode == ALLEGRO_KEY_Q) 		joystick_punch(player_1->control);						
						else if (event.keyboard.keycode == ALLEGRO_KEY_R) 		joystick_kick(player_1->control);
						

						if (!player_2->is_bot){
							if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) 		joystick_left(player_2->control);																											
							else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) 	joystick_right(player_2->control);																																																											
							else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) 	joystick_crouch(player_2->control);
							else if (event.keyboard.keycode == ALLEGRO_KEY_UP) 		joystick_jump(player_2->control);
							else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2) 	joystick_parry(player_2->control);
							else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_0) 	joystick_punch(player_2->control);
							else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) 	joystick_kick(player_2->control);
						}

					}
					else if (event.type == 42) return 0;																    
				}
			}
			rounds++;
			printf("round\n");
			square_destroy(player_1);																																												
			square_destroy(player_2);

		}

		double start_time = 0;
		/* EXIBIÇÂO DO VENCEDOR */
		al_clear_to_color(al_map_rgb(0, 0, 0));
		if (p1_score > p2_score) {
			character1->current_status = VICTORY;
			start_time = al_get_time();
		} else {
			character2->current_status = VICTORY;
			start_time = al_get_time();
		}

		al_flip_display();
		ALLEGRO_EVENT event;
		double last_time = al_get_time();
		double current_time;
		float delta_time;
		while (1) {
			current_time = al_get_time();
			delta_time = (float)(current_time - last_time);
			last_time = current_time;

 			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_scaled_bitmap(background, 
				0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 
				0, 0, X_SCREEN, Y_SCREEN, 
				0);

			// Atualizar a animação de vitória
			if (p1_score > p2_score) {
				draw_animation(character1, X_SCREEN / 2, FLOOR - 120, 1, delta_time);
				char name [20];
				strcpy(name, characters[0]->name);
				al_draw_text(font, al_map_rgb(255, 140, 0), X_SCREEN / 2, Y_SCREEN / 2 - 40, ALLEGRO_ALIGN_CENTRE, name);
			} else {
				draw_animation(character2, X_SCREEN / 2, FLOOR - 120, 1, delta_time);
				char name [20];
				strcpy(name, characters[1]->name);
				al_draw_text(font, al_map_rgb(255, 140, 0), X_SCREEN / 2, Y_SCREEN / 2 - 40, ALLEGRO_ALIGN_CENTRE, name);
			}

			al_flip_display();

			if (current_time - start_time > 5.0) { 
				break;
			}

		}

		//ALLEGRO_EVENT event;

		/* PERGUNTA SE QUER JOGAR NOVAMENTE */
		al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2 + 40, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para jogar novamente ou ESC para sair");
        al_flip_display();

        
        int decision_made = 0;
        while (!decision_made) {
            al_wait_for_event(queue, &event);
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    play_again = 1;
                    decision_made = 1;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    play_again = 0;
                    decision_made = 1;
                }
            }
        }
	}

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);


    return 0;
}
