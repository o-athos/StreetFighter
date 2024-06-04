/* 
Compilação: 
gcc streetFighter.c square.c joystick.c -o streetFighter $(pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5)
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

#define X_SCREEN 1000
#define Y_SCREEN 500

unsigned char collision_2D(square *element_first, square *element_second) {
    float half_side_first = element_first->side / 2.0;
    float half_side_second = element_second->side / 2.0;
    
    float left_first = element_first->x - half_side_first;
    float right_first = element_first->x + half_side_first;
    float top_first = element_first->y + half_side_first;
    float bottom_first = element_first->y - half_side_first;

    float left_second = element_second->x - half_side_second;
    float right_second = element_second->x + half_side_second;
    float top_second = element_second->y + half_side_second;
    float bottom_second = element_second->y - half_side_second;
    
    if (left_first < right_second && right_first > left_second &&
        bottom_first < top_second && top_first > bottom_second) {
        return 1;
    } else {
        return 0;
    }
}


 void update_position(square *player_1, square *player_2){																																				//Função de atualização das posições dos quadrados conforme os comandos do controle (!)
    
	if (player_1->control->left){																																										//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado... (!)
		square_move(player_1, 1, 0, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para a esquerda (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 0, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	if (player_1->control->right){																																										//Se o botão de movimentação para direita do controle do primeir ojogador está ativado... (!)
		square_move(player_1, 1, 1, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para a direta (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 1, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	if (player_1->control->up) {																																										//Se o botão de movimentação para cima do controle do primeiro jogador está ativado... (!)
		square_move(player_1, 1, 2, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para cima (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 2, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	if (player_1->control->down){																																										//Se o botão de movimentação para baixo do controle do primeiro jogador está ativado... (!)
		square_move(player_1, 1, 3, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do primeiro jogador para a baixo (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 3, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}

	if (player_2->control->left){																																										//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 0, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 0, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	
	if (player_2->control->right){ 																																										//Se o botão de movimentação para direita do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 1, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a direita (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 1, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	
	if (player_2->control->up){																																											//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 2, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a cima (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 2, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)			
	}
	if (player_2->control->down){																																										//Se o botão de movimentação para baixo do controle do segundo jogador está ativado... (!)
		square_move(player_2, 1, 3, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a baixo (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 3, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
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






    square* player_1 = square_create(20,  10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);				//Cria o quadrado do primeiro jogador
	if (!player_1) return 1;																    //Verificação de erro na criação do quadrado do primeiro jogador
	square* player_2 = square_create(20, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);		//Cria o quadrado do segundo jogador
	if (!player_2) return 2;


    ALLEGRO_EVENT event;
    while(1){																																															//Laço principal do programa
		al_wait_for_event(queue, &event);																																								//Função que captura eventos da fila, inserindo os mesmos na variável de eventos


        if (event.type == 30){
            update_position(player_1, player_2);																																						//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
            al_clear_to_color(al_map_rgb(0, 0, 0));																																						//Verifica se o segundo jogador matou o primeiro jogador																																						//Substitui tudo que estava desenhado na tela por um fundo preto
            al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->side/2, player_1->x+player_1->side/2, player_1->y+player_1->side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
            al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->side/2, player_2->x+player_2->side/2, player_2->y+player_2->side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela																																			
            al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
        }
        else if ((event.type == 10) || (event.type == 12)){																																				
            if (event.keyboard.keycode == 1) joystick_left(player_1->control);				//a																											
            else if (event.keyboard.keycode == 4) joystick_right(player_1->control);		//d																											
            else if (event.keyboard.keycode == 23) joystick_up(player_1->control);			//w																											
            else if (event.keyboard.keycode == 19) joystick_down(player_1->control);		//s																											
            else if (event.keyboard.keycode == 82) joystick_left(player_2->control);		//seta esq																										
            else if (event.keyboard.keycode == 83) joystick_right(player_2->control);		//seta dir																											
            else if (event.keyboard.keycode == 84) joystick_up(player_2->control);			//seta cima																											
            else if (event.keyboard.keycode == 85) joystick_down(player_2->control);		//seta baixo																																																						    //Indica o evento correspondente no controle do segundo joagdor (botão de disparo - shift dir)
        }
        else if (event.type == 42) break;
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
	square_destroy(player_1);																																												
	square_destroy(player_2);	
    																																											
    return 0;
}
