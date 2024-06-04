#include "joystick.h"
#include <stdlib.h>

joystick* joystick_create(){

    joystick* elem = (joystick*)malloc(sizeof(joystick));
    if (!elem) return NULL;
    elem->right = 0;																
	elem->left = 0;																
	elem->up = 0;																
	elem->down = 0;	
    elem->fire = 0;															
	return elem;																	
}

void joystick_destroy(joystick* elem){
    free(elem);
}

void joystick_left(joystick* elem){
    elem->left = elem->left ^ 1;
}

void joystick_right(joystick* elem){
    elem->right = elem->right ^ 1;
}

void joystick_up(joystick* elem){
    elem->up = elem->up ^ 1;
}

void joystick_down(joystick* elem){
    elem->down = elem->down ^ 1;
}

void joystick_fire(joystick* elem){
    elem->fire = elem->fire ^ 1;
}