#include <stdlib.h>
#include <stdio.h>
#include "square.h"

square* square_create(unsigned int side, unsigned char face, unsigned int x, unsigned int y, unsigned int x_max, unsigned int y_max){

    if (x - side/2 < 0 || x + side/2 > x_max || y - side/2 < 0 || y + side/2 > y_max) {
        printf("Não foi possível criar elemento nessa posição\n");
        return NULL;
    }

    square* new_element  = (square*)malloc(sizeof(square));
    new_element->side = side;
    new_element->face = face;
    new_element->x = x;
    new_element->y = y;
    new_element->control = joystick_create();
    new_element->gun = pistol_create();

    return new_element;
}

void square_move(square* elem, char steps, unsigned char trajectory, unsigned int x_max, unsigned int y_max){

    int step_size = steps * SQUARE_STEP;

    if (trajectory == 0){  // esquerda
        if (((elem->x - elem->side/2) - step_size) >= 0) 
            elem->x = elem->x - step_size; 
    }
    
    if (trajectory == 1){   // direita
        if (((elem->x + elem->side/2) + step_size) <= x_max)
            elem->x = elem->x + step_size;
    }

    if (trajectory == 2){   // cima
        if (((elem->y - elem->side/2) - step_size) >= 0)
            elem->y = elem->y - step_size;
    }

    if (trajectory == 3){   //baixo
        if (((elem->y + elem->side/2) + step_size) <= y_max)
            elem->y = elem->y + step_size;
    }
}

void square_shot(square *element){
    bullet *shot;

	if (!element->face) 
        shot = pistol_shot(element->x - element->side/2, element->y, element->face, element->gun);										
	else if 
        (element->face == 1) shot = pistol_shot(element->x + element->side/2, element->y, element->face, element->gun);		

	if (shot) element->gun->shots = shot;
}

void square_destroy(square* elem){

    pistol_destroy(elem->gun);																														
	joystick_destroy(elem->control);	
    free(elem);
}