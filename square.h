#ifndef __SQUARE__ 																												//Guardas de inclusão																											//Guardas de inclusão																										//Quantidade de pixels que um quadrado se move por passo
#define __SQUARE__

#include "joystick.h"																											//Estrutura e procedimentos relacionados ao controle do quadrado (!)

#define SQUARE_STEP 10																											//Tamanho, em pixels, de um passo do quadrado

typedef struct {																												//Definição da estrutura de um quadrado
	unsigned char side;																											//Tamanmho da lateral de um quadrado
	unsigned short x;																											//Posição X do centro do quadrado
	unsigned short y;																											//Posição Y do centro do quadrado
	joystick *control;																											//Elemento de controle do quadrado no jogo (!)	

} square;																														//Definição do nome da estrutura

square* square_create(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);		//Protótipo da função de criação de um quadrado
void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);			//Protótipo da função de movimentação de um quadrado (!)
void square_destroy(square *element);																							//Protótipo da função de destruição de um quadrado

#endif																															//Guardas de inclusão