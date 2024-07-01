/* 
Compilação: 
gcc streetFighter.c square.c joystick.c pistol.c bullet.c -o streetFighter $(pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5)
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

#define X_SCREEN 1000
#define Y_SCREEN 500
#define FLOOR 400
#define HIGH_JUMP 325
#define JUMP_SPEED 10
#define GRAVITY 5

unsigned char collision_2D(square *element_first, square *element_second) {
    
	float half_x_side_first = element_first->x_side / 2.0;
    float half_y_side_first = element_second->y_side / 2.0;

    float half_x_side_second = element_second->x_side / 2.0;
    float half_y_side_second = element_second->y_side / 2.0;

    float left_first = element_first->x - half_x_side_first;
    float right_first = element_first->x + half_x_side_first;
    float top_first = element_first->y + half_y_side_first;
    float bottom_first = element_first->y - half_y_side_first;

    float left_second = element_second->x - half_x_side_second;
    float right_second = element_second->x + half_x_side_second;
    float top_second = element_second->y + half_y_side_second;
    float bottom_second = element_second->y - half_y_side_second;
    if (left_first < right_second && right_first > left_second &&
        bottom_first < top_second && top_first > bottom_second) {
        return 1;
    } else {
        return 0;
    }
}

void update_bullets(square* player){

    bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index = player->gun->shots; index != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		if (!index->trajectory) index->x -= BULLET_MOVE;																																					//Se a trajetória for para a esquerda, atualiza a posição para a esquerda (!)
		else if (index->trajectory == 1) index->x += BULLET_MOVE;																																			//Se a trajetória for para a direita, atualiza a posição para a esquerda (!)
		
		if ((index->x < 0) || (index->x > X_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela (!)
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis (!)
				previous->next = index->next;																																								//Se não for, salva o próximo projétil (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = (bullet*) previous->next;																																							//Atualiza para o próximo projétil (!)
			}
			else {																																															//Se for o primeiro projétil da lista (!)
				player->gun->shots = (bullet*) index->next;																																					//Atualiza o projétil no início da lista (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = player->gun->shots;																																									//Atualiza para o próximo projétil (!)
			}
		}
		else{																																																//Se não saiu da tela (!)
			previous = index;																																												//Atualiza o projétil anterior (para a próxima iteração) (!)
			index = (bullet*) index->next;																																									//Atualiza para o próximo projétil (!)
		}
	}
}

void update_position(square *player_1, square *player_2){																																				
    
	if (player_1->control->left){																																										//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado... (!)
		square_move(player_1, 1, 0, X_SCREEN, FLOOR);																																				//Move o quadrado do primeiro jogador para a esquerda (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 0, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
		player_1->face = 0;
	}
	if (player_1->control->right){																																										//Se o botão de movimentação para direita do controle do primeir ojogador está ativado... (!)
		square_move(player_1, 1, 1, X_SCREEN, FLOOR);																																				//Move o quadrado do primeiro jogador para a direta (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 1, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
		player_1->face = 1;
	}
	if (player_1->control->up) {																																										//Se o botão de movimentação para cima do controle do primeiro jogador está ativado... (!)
		square_move(player_1, 1, 2, X_SCREEN, FLOOR);																																				//Move o quadrado do primeiro jogador para cima (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 2, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	if (player_1->control->down && !player_1->is_jump && !player_1->is_faling){																																										//Se o botão de movimentação para baixo do controle do primeiro jogador está ativado... (!)
		square_move(player_1, 1, 3, X_SCREEN, FLOOR);																																				//Move o quadrado do primeiro jogador para a baixo (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 3, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}

	if (player_2->control->left){																																										//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 0, X_SCREEN, FLOOR);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 0, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
		player_2->face = 0;
	}
	
	if (player_2->control->right){ 																																										//Se o botão de movimentação para direita do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 1, X_SCREEN, FLOOR);																																				//Move o quadrado do segundo jogador para a direita (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 1, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
		player_2->face = 1;
	}
	
	if (player_2->control->up){																																											//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 2, X_SCREEN, FLOOR);																																				//Move o quadrado do segundo jogador para a cima (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 2, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)			
	}
	if (player_2->control->down && !player_2->is_jump && !player_2->is_faling){																																										//Se o botão de movimentação para baixo do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 3, X_SCREEN, FLOOR);																																				//Move o quadrado do segundo jogador para a baixo (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 3, X_SCREEN, FLOOR);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}



	/* TIRO */
    if (player_1->control->fire){																																											//Verifica se o primeiro jogador está atirando (!)
		if (!player_1->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
			square_shot(player_1); 																																											//Se não estiver, faz um disparo (!)
			player_1->gun->timer = PISTOL_FIRE_RATE;																																							//Inicia o cooldown da arma (!)
		} 
	}

	if (player_2->control->fire){																																											//Verifica se o segundo jogador está atirando (!)
		if (!player_2->gun->timer){																																											//Verifica se a arma do segundo jogador não está em cooldown (!)
			square_shot(player_2);																																											//Se não estiver, faz um disparo (!)
			player_2->gun->timer = PISTOL_FIRE_RATE;																																							//Inicia o cooldown da arma (!)
		}
	}
	update_bullets(player_1);																																												//Atualiza os disparos do primeiro jogador (!)
	update_bullets(player_2);




	/* PULO DO PLAYER 1 */
	if (player_1->control->jump){
		square_jump(player_1, FLOOR);
	}
	
	if (player_1->is_jump){
		player_1->y -= JUMP_SPEED;
		if (player_1->y <= HIGH_JUMP){
			player_1->is_faling = 1;
			player_1->is_jump = 0;
		}
	}
	if (collision_2D(player_1, player_2)){
		player_1->y += JUMP_SPEED;
		player_1->is_jump = 0;
	}
	if (player_1->is_faling){
		player_1->y += GRAVITY;
		if (player_1->y >= FLOOR - 10){
			player_1->is_faling = 0;
		}
	}
	if (collision_2D(player_1, player_2)){
		player_1->y -= GRAVITY;
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
	if (collision_2D(player_2, player_1)){
		player_2->y += JUMP_SPEED;
		player_2->is_jump = 0;
	}
	if (player_2->is_faling){
		player_2->y += GRAVITY;
		if (player_2->y >= FLOOR - 10){
			player_2->is_faling = 0;
		}
	}
	if (collision_2D(player_2, player_1)){
		player_2->y -= GRAVITY;
	}

	/* SE ABAIXA */
	if (player_1->control->crouch){
		square_crouch(player_1);
	} else {
		square_stand(player_1);
	}
    if (player_2->control->crouch) {
        square_crouch(player_2);
    } else {
        square_stand(player_2);
    }


	/* SOCO  */
	if (player_1->control->punch){
		if (!player_1->punch_timer){
			square_punch(player_1, player_2);
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

    // Variável para controlar o estado do menu
    bool menu = true;

   // Loop principal do jogo
    while (menu) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Desenhar o menu na tela
        al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpar a tela

        al_draw_text(font, al_map_rgb(255, 165, 0), 500, 200, ALLEGRO_ALIGN_CENTER, "Street Fighter"); // Título do jogo
        al_draw_text(font, al_map_rgb(255, 255, 255), 500, 300, ALLEGRO_ALIGN_CENTER, "Pressione Enter para jogar"); // Mensagem para pressionar Enter

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

	int p1_score = 0, p2_score = 0;
	int rounds = 0;
	while (p1_score - p2_score != 2 && p2_score - p1_score != 2 && rounds < 3){

		square* player_1 = square_create(25, 40, 1, 50, FLOOR-10, X_SCREEN, Y_SCREEN);				//Cria o quadrado do primeiro jogador
		if (!player_1) return 1;																//Verificação de erro na criação do quadrado do primeiro jogador
		square* player_2 = square_create(25, 40, 0, X_SCREEN-50, FLOOR-10, X_SCREEN, Y_SCREEN);		//Cria o quadrado do segundo jogador
		if (!player_2) return 2;

		player_1->health_bar = create_health_bar(10, 10, 400, 5, player_1->hp);
		player_2->health_bar = create_health_bar(X_SCREEN - 410, 10, 400, 5, player_2->hp);

		printf("p1:%d p2:%d\n", p1_score, p2_score);

		unsigned char p1_isDead = 0, p2_isDead = 0, round_over = 0;
		ALLEGRO_EVENT event;
		if (event.type == 42) break;
		while(1){																																															//Laço principal do programa
			al_wait_for_event(queue, &event);																																								//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
			
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
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 175, Y_SCREEN/2+20, 0, "PRESSIONE ENTER PARA SAIR");																					
				al_flip_display();																																												

				//if ((event.type == 10) && (event.keyboard.keycode == ALLEGRO_KEY_ENTER)) break;																																
				//else if (event.type == 42) break;
				ALLEGRO_EVENT event;
    			while (1) {
        			al_wait_for_event(queue, &event);
        			if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            			round_over = 1;
						break;
        			}
    			}		
			}
			else {
				if (event.type == 30){
					update_position(player_1, player_2);	
					
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

					for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet *)index->next){
						al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));
					}
					if (player_1->gun->timer) player_1->gun->timer--;

					for (bullet *index = player_2->gun->shots; index != NULL; index = (bullet *)index->next){
						al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));
					}
					if (player_2->gun->timer) player_2->gun->timer--;


					if (player_1->punch_timer) player_1->punch_timer--;
					if (player_2->punch_timer) player_2->punch_timer--;	

					if (player_1->kick_timer) player_1->kick_timer--;
					if (player_2->kick_timer) player_2->kick_timer--;	

					al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
				}
				else if ((event.type == 10) || (event.type == 12)){	

					if (event.keyboard.keycode == ALLEGRO_KEY_A) joystick_left(player_1->control);																															
					else if (event.keyboard.keycode == ALLEGRO_KEY_D) joystick_right(player_1->control);																													
					//else if (event.keyboard.keycode == ALLEGRO_KEY_W) joystick_up(player_1->control);																													
					else if (event.keyboard.keycode == ALLEGRO_KEY_S) joystick_crouch(player_1->control);		

					else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) joystick_left(player_2->control);																											
					else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) joystick_right(player_2->control);																													
					//else if (event.keyboard.keycode == ALLEGRO_KEY_UP) joystick_up(player_2->control);																														
					else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) joystick_crouch(player_2->control);

					else if (event.keyboard.keycode == ALLEGRO_KEY_C)	joystick_fire(player_1->control);
					else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) joystick_fire(player_2->control);

					else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) joystick_jump(player_1->control);
					else if (event.keyboard.keycode == ALLEGRO_KEY_UP) joystick_jump(player_2->control);

					else if (event.keyboard.keycode == ALLEGRO_KEY_E) joystick_parry(player_1->control);
					else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_0) joystick_parry(player_2->control);
					
					else if (event.keyboard.keycode == ALLEGRO_KEY_Q) joystick_punch(player_1->control);
					else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_4) joystick_punch(player_2->control);

					else if (event.keyboard.keycode == ALLEGRO_KEY_X) joystick_kick(player_1->control);
					else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_DELETE) joystick_kick(player_2->control);
				}
				else if (event.type == 42) break;																    
			}
		}
		rounds++;
		printf("round\n");
		square_destroy(player_1);																																												
		square_destroy(player_2);

	}


    /* EXIBIÇÂO DO VENCEDOR */
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (p1_score > p2_score) {
        al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Player 1 Wins!");
    } else {
        al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTRE, "Player 2 Wins!");
    }
    al_flip_display();

	ALLEGRO_EVENT event;
	while (1) {
    	al_wait_for_event(queue, &event);
    	if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
        	break;
    	}
	}

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);


    return 0;
}
