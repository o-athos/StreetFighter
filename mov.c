#include "mov.h"

#define X_SCREEN 1000
#define Y_SCREEN 500 

unsigned char collision_2D(square *element_first, square *element_second){

	if ((((element_first->y+element_first->side/2 >= element_second->y-element_second->side/2) && (element_second->y-element_second->side/2 >= element_first->y-element_first->side/2)) || 	//			//Verifica se o primeiro elemento colidiu com o segundo no eixo X			
		((element_second->y+element_second->side/2 >= element_first->y-element_first->side/2) && (element_first->y-element_first->side/2 >= element_second->y-element_second->side/2))) && 	//			//Verifica se o segundo elemento colidiu com o primeiro no eixo X
		(((element_first->x+element_first->side/2 >= element_second->x-element_second->side/2) && (element_second->x-element_second->side/2 >= element_first->x-element_first->side/2)) || 	//			//Verifica se o primeiro elemento colidiu com o segundo no eixo Y
		((element_second->x+element_second->side/2 >= element_first->x-element_first->side/2) && (element_first->x-element_first->side/2 >= element_second->x-element_second->side/2)))) return 1;		//Verifica se o segundo elemento colidiu com o primeiro no eixo Y
	else return 0;
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