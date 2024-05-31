/* 
Compilação: 
gcc streetFighter.c square.c joystick.c mov.c -o streetFighter $(pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5)
*/

#include <stdio.h>

//bibliotecas allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

//bibliotecas pessoais
#include "square.h"
#include "joystick.h"
#include "mov.h"

#define X_SCREEN 1000
#define Y_SCREEN 500

int main() {
    // Inicializar Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro!\n");
        return -1;
    }

    al_init_primitives_addon();

    // Inicializar teclado
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado!\n");
        return -1;
    }

    // Inicializar add-ons
    al_init_font_addon();
    al_init_ttf_addon();
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar add-on de imagem!\n");
        return -1;
    }

    // Criar temporizador, fila de eventos e display
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_FONT *font = al_load_ttf_font("/usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf", 30, 0);

    // Verificar se os componentes foram criados com sucesso
    if (!timer || !queue || !disp || !font) {
        if (!timer) fprintf(stderr, "Falha ao criar temporizador!\n");
        if (!queue) fprintf(stderr, "Falha ao criar fila de eventos!\n");
        if (!disp) fprintf(stderr, "Falha ao criar display!\n");
        if (!font) fprintf(stderr, "Falha ao carregar a fonte!\n");
        return -1;
    }

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

    square* player_1 = square_create(20, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);				//Cria o quadrado do primeiro jogador
	if (!player_1) return 1;																//Verificação de erro na criação do quadrado do primeiro jogador
	square* player_2 = square_create(20, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);		//Cria o quadrado do segundo jogador
	if (!player_2) return 2;

    ALLEGRO_EVENT event;
    while(1){																																															//Laço principal do programa
		al_wait_for_event(queue, &event);																																								//Função que captura eventos da fila, inserindo os mesmos na variável de eventos

		if (event.type == 30){
			update_position(player_1, player_2);																																						//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
			al_clear_to_color(al_map_rgb(0, 0, 0));																																						//Substitui tudo que estava desenhado na tela por um fundo preto
			al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->side/2, player_1->x+player_1->side/2, player_1->y+player_1->side/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
			al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->side/2, player_2->x+player_2->side/2, player_2->y+player_2->side/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela
    		al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
        }
		else if ((event.type == 10) || (event.type == 12)){																																				//Verifica se o evento é de botão do teclado abaixado ou levantado (!)
			if (event.keyboard.keycode == 1) joystick_left(player_1->control);																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda) (!)
			else if (event.keyboard.keycode == 4) joystick_right(player_1->control);																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita) (!)
			else if (event.keyboard.keycode == 23) joystick_up(player_1->control);																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima) (!)
			else if (event.keyboard.keycode == 19) joystick_down(player_1->control);																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para baixo) (!)
			else if (event.keyboard.keycode == 82) joystick_left(player_2->control);																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à esquerda) (!)
			else if (event.keyboard.keycode == 83) joystick_right(player_2->control);																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita) (!)
			else if (event.keyboard.keycode == 84) joystick_up(player_2->control);																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima) (!)
			else if (event.keyboard.keycode == 85) joystick_down(player_2->control);																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para baixo) (!)
		}
        else if (event.type == 42) break;
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
