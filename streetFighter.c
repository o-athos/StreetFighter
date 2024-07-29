/* 
Compilação: 
gcc streetFighter.c square.c joystick.c pistol.c bullet.c health_bar.c character_selection.c -o streetFighter $(pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5)
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
#include "pistol.h"
#include "bullet.h"
#include "character_selection.h"
#include "sprites.h"

#define X_SCREEN 1000
#define Y_SCREEN 500
#define FLOOR 400
#define HIGH_JUMP 300
#define JUMP_SPEED 10
#define GRAVITY 5



void update_bullets(square* player){

    bullet *previous = NULL;																																												
	for (bullet *index = player->gun->shots; index != NULL;){																																				
		if (!index->trajectory) index->x -= BULLET_MOVE;																																					
		else if (index->trajectory == 1) index->x += BULLET_MOVE;																																			
		
		if ((index->x < 0) || (index->x > X_SCREEN)){																																						
			if (previous){																																													
				previous->next = index->next;																																								
				bullet_destroy(index);																																										
				index = (bullet*) previous->next;																																							
			}
			else {																																															
				player->gun->shots = (bullet*) index->next;																																					
				bullet_destroy(index);																																									
				index = player->gun->shots;																																									
			}
		}
		else{																																																
			previous = index;																																												
			index = (bullet*) index->next;																																									
		}
	}
}

void update_position(square *player_1, square *player_2){																																				
	
	if (player_1->control->left && !player_1->is_crouching){																																										
		square_move(player_1, 1, 0, X_SCREEN, FLOOR);																																																												
			player_1->face = 0;
	}
	if (player_1->control->right && !player_1->is_crouching){																																										
		square_move(player_1, 1, 1, X_SCREEN, FLOOR);																																				
		player_1->face = 1;
	}
	if (player_1->control->up) {																																										
		square_move(player_1, 1, 2, X_SCREEN, FLOOR);																																																															
	}
	if (player_1->control->down && !player_1->is_jump && !player_1->is_faling){																																										
		square_move(player_1, 1, 3, X_SCREEN, FLOOR);																																																												
	}

	if (player_2->control->left && !player_2->is_crouching){
		square_move(player_2, 1, 0, X_SCREEN, FLOOR);																																																															
		player_2->face = 0;
	}
	
	if (player_2->control->right && !player_2->is_crouching){ 																																										
		square_move(player_2, 1, 1, X_SCREEN, FLOOR);
		player_2->face = 1;
	}
	
	if (player_2->control->up){																																											
		square_move(player_2, 1, 2, X_SCREEN, FLOOR);																													
	}
	if (player_2->control->down && !player_2->is_jump && !player_2->is_faling){																																										
		square_move(player_2, 1, 3, X_SCREEN, FLOOR);																																																													
	}



	/* TIRO */
    if (player_1->control->fire){																																											
		if (!player_1->gun->timer){																																											
			square_shot(player_1); 																																											
			player_1->gun->timer = PISTOL_FIRE_RATE;																																							
		} 
	}

	if (player_2->control->fire){																																											
		if (!player_2->gun->timer){																																											
			square_shot(player_2);																																											
			player_2->gun->timer = PISTOL_FIRE_RATE;																																							
		}
	}
	update_bullets(player_1);																																												
	update_bullets(player_2);



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
	if (player_1->control->punch){
		if (!player_1->punch_timer){
			square_punch(player_1, player_2);
			player_1->is_punching = 1;
			player_1->punch_timer = PUNCH_COOLDOWN;
		}
	}
	if (player_2->control->punch){
		if (!player_2->punch_timer){
			square_punch(player_2, player_1);

			player_2->punch_timer = PUNCH_COOLDOWN;
		}
	}

	/* CHUTE */
	if (player_1->control->kick){
		if (!player_1->kick_timer){
			square_kick(player_1, player_2);
			player_1->kick_timer = KICK_COOLDOWN;
		}
	}
	if (player_2->control->kick){
		if (!player_2->kick_timer){
			square_kick(player_2, player_1);

			player_2->kick_timer = KICK_COOLDOWN;
		}
	}
}

unsigned char check_kill(square *killer, square *victim){																																					

	bullet *previous = NULL;
	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){																													
		if ((index->x >= victim->x - victim->x_side/2) && (index->x <= victim->x + victim->x_side/2) && //																										
		   (index->y >= victim->y - victim->y_side/2) && (index->y <= victim->y + victim->y_side/2)){	
			if (!victim->control->parry)																								
				victim->hp--;																																													
			if (victim->hp){																																												
				if (previous){																																												
					previous->next = index->next;																																							
					bullet_destroy(index);																																									
					index = (bullet*) previous->next;																																						
				}
				else {																																														
					killer->gun->shots = (bullet*) index->next;																																			
					bullet_destroy(index);																																									
					index = killer->gun->shots;																																								
				}
				return 0;																																													
			}
			else return 1;																																													
		}
		previous = index;																																													
	}
	return 0;																																																
}



int main() {

    al_init();

    al_init_primitives_addon();

    al_install_keyboard();

    al_init_font_addon();
    al_init_ttf_addon();

    al_init_image_addon();
        


    // Criar temporizador, fila de eventos e display
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_FONT *font = al_load_ttf_font("fonts/PixelifySans-VariableFont_wght.ttf", 30, 0);


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


	/* -------------------------INICIO DO JOGO COM ROUNDS --------------------------------*/


	int play_again = 1;
	while (play_again){
	
		character *characters[2];
    	choose_character(queue, font, characters, 1);
    	choose_character(queue, font, characters, 2);

		Character* character1 = load_character(characters[0]->spritesheet_path, 5, 4, 1, 5, 3, 7, 1, 1, 3, 100, 120);

		int p1_score = 0, p2_score = 0;
		int rounds = 0;
		while (p1_score - p2_score != 2 && p2_score - p1_score != 2 && rounds < 3){

			square* player_1 = square_create(25, 40, 1, 50, FLOOR-20, X_SCREEN, Y_SCREEN);				//Cria o quadrado do primeiro jogador
			if (!player_1) return 1;																//Verificação de erro na criação do quadrado do primeiro jogador
			square* player_2 = square_create(25, 40, 0, X_SCREEN-50, FLOOR-20, X_SCREEN, Y_SCREEN);		//Cria o quadrado do segundo jogador
			if (!player_2) return 2;

			player_1->health_bar = create_health_bar(10, 10, 400, 15, player_1->hp);
			player_2->health_bar = create_health_bar(X_SCREEN - 410, 10, 400, 15, player_2->hp);

			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_filled_rectangle(player_1->x-player_1->x_side/2, player_1->y-player_1->y_side/2, player_1->x+player_1->x_side/2, player_1->y+player_1->y_side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
			al_draw_filled_rectangle(player_2->x-player_2->x_side/2, player_2->y-player_2->y_side/2, player_2->x+player_2->x_side/2, player_2->y+player_2->y_side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela

			draw_health_bar(disp, player_1->health_bar);
			draw_health_bar(disp, player_2->health_bar);

			draw_animation(character1, player_1->x-2*player_1->x_side, player_1->y-player_1->y_side, player_1->face, 0);

			char round_message[20];

			sprintf(round_message, "Round %d", rounds + 1);
			//al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, round_message);
			al_flip_display();
			al_rest(1.0);

			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_filled_rectangle(player_1->x-player_1->x_side/2, player_1->y-player_1->y_side/2, player_1->x+player_1->x_side/2, player_1->y+player_1->y_side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
			al_draw_filled_rectangle(player_2->x-player_2->x_side/2, player_2->y-player_2->y_side/2, player_2->x+player_2->x_side/2, player_2->y+player_2->y_side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela

			draw_health_bar(disp, player_1->health_bar);
			draw_health_bar(disp, player_2->health_bar);

			draw_animation(character1, player_1->x-2*player_1->x_side, player_1->y-player_1->y_side, player_1->face, 0);

			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Fight!");
			al_flip_display();
			al_rest(1.0);	

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
						al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 90, Y_SCREEN/2-15, 0, "JOGADOR 1 GANHOU!");																				
					}
					else if (p1_isDead){
						p2_score++;
						al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 - 90, Y_SCREEN/2-15, 0, "JOGADOR 2 GANHOU!");
					}																																								
					al_flip_display();																																												

					al_rest(3.0);

					round_over = 1;
				}
				else {
					if (event.type == 30){

						update_position(player_1, player_2);	

						update_character_status(character1, player_1);

						p1_isDead = check_kill(player_2, player_1);
						p2_isDead = check_kill(player_1, player_2);
						if (player_1->hp == 0)
							p1_isDead = 1;
						if (player_2->hp == 0)
							p2_isDead = 1;
																																										
						al_clear_to_color(al_map_rgb(0, 0, 0));																																																																									//Substitui tudo que estava desenhado na tela por um fundo preto
						al_draw_filled_rectangle(player_1->x-player_1->x_side/2, player_1->y-player_1->y_side/2, player_1->x+player_1->x_side/2, player_1->y+player_1->y_side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
						al_draw_filled_rectangle(player_2->x-player_2->x_side/2, player_2->y-player_2->y_side/2, player_2->x+player_2->x_side/2, player_2->y+player_2->y_side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela
						
						draw_health_bar(disp, player_1->health_bar);
						draw_health_bar(disp, player_2->health_bar);

						draw_animation(character1, player_1->x-2*player_1->x_side, player_1->y-player_1->y_side, player_1->face, delta_time);

						if (p1_score == 1)
							al_draw_filled_circle(15, 25, 5, al_map_rgb(255, 0, 0));
						if (p2_score == 1){
							al_draw_filled_circle(X_SCREEN - 15, 25, 5, al_map_rgb(0, 0, 255));
						}


						for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet *)index->next){
							al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));
						}
						if (player_1->gun->timer) player_1->gun->timer--;

						for (bullet *index = player_2->gun->shots; index != NULL; index = (bullet *)index->next){
							al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));
						}
						if (player_2->gun->timer) player_2->gun->timer--;


						if (player_1->punch_timer) {
							player_1->punch_timer--;
						}
						else{
							player_1->is_punching = 0;
						}
						if (player_2->punch_timer) player_2->punch_timer--;	

						if (player_1->kick_timer) player_1->kick_timer--;
						if (player_2->kick_timer) player_2->kick_timer--;	

						al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
					}
					else if ((event.type == 10) || (event.type == 12)){	

						if (event.type == 10){
							if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
								pause = !pause;
							}
						}

						if (event.keyboard.keycode == ALLEGRO_KEY_A) joystick_left(player_1->control);																															
						else if (event.keyboard.keycode == ALLEGRO_KEY_D) joystick_right(player_1->control);							
						//else if (event.keyboard.keycode == ALLEGRO_KEY_W) joystick_up(player_1->control);																													
						else if (event.keyboard.keycode == ALLEGRO_KEY_S) joystick_crouch(player_1->control);		

						else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) joystick_left(player_2->control);																											
						else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) joystick_right(player_2->control);																													
						//else if (event.keyboard.keycode == ALLEGRO_KEY_UP) joystick_up(player_2->control);																														
						else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) joystick_crouch(player_2->control);

						else if (event.keyboard.keycode == ALLEGRO_KEY_C)	joystick_fire(player_1->control);
						else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_4) joystick_fire(player_2->control);

						else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) joystick_jump(player_1->control);
						else if (event.keyboard.keycode == ALLEGRO_KEY_UP) joystick_jump(player_2->control);

						else if (event.keyboard.keycode == ALLEGRO_KEY_E) joystick_parry(player_1->control);
						else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2) joystick_parry(player_2->control);
						
						else if (event.keyboard.keycode == ALLEGRO_KEY_Q) joystick_punch(player_1->control);
						else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_0) joystick_punch(player_2->control);

						else if (event.keyboard.keycode == ALLEGRO_KEY_R) joystick_kick(player_1->control);
						else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) joystick_kick(player_2->control);
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
			///al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Player 1 Wins!");
		} else {
			//al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Player 2 Wins!");
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
			// Atualizar a animação de vitória
			if (p1_score > p2_score) {
				draw_animation(character1, X_SCREEN / 2, Y_SCREEN / 2, 1, delta_time);
				char name [20];
				strcpy(name, characters[0]->name);
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTRE, name);
			} else {
				//draw_animation(character2, X_SCREEN / 2, Y_SCREEN / 2, delta_time);
			}

			al_flip_display();

			if (current_time - start_time > 50.0) { 
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
