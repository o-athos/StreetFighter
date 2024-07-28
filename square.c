#include <stdlib.h>
#include <stdio.h>
#include "square.h"
#include "health_bar.h"

square* square_create(unsigned char x_side, unsigned char y_side, unsigned char face, unsigned short x, unsigned short y, unsigned short x_max, unsigned int y_max){

    if (x - x_side/2 < 0 || x + x_side/2 > x_max || y - y_side/2 < 0 || y + y_side/2 > y_max) {
        printf("Não foi possível criar elemento nessa posição\n");
        return NULL;
    }

    square* new_element  = (square*)malloc(sizeof(square));
    if (!new_element) return NULL;
    new_element->x_side = x_side;
    new_element->y_side = y_side;
    new_element->face = face;
    new_element->x = x;
    new_element->y = y;
    new_element->hp = 10;
    new_element->is_jump = 0;
    new_element->is_faling = 0;
    new_element->is_crouching = 0;
    new_element->is_kicking = 0;
    new_element->is_punching = 0;
    new_element->punch_timer = 0;
    new_element->kick_timer = 0;   
    new_element->control = joystick_create();
    new_element->gun = pistol_create();

    return new_element;
}

void square_move(square* elem, char steps, unsigned char trajectory, unsigned short x_max, unsigned short y_max){

    int step_size = steps * SQUARE_STEP;

    if (trajectory == 0){  // esquerda
        if (((elem->x - elem->x_side/2) - step_size) >= 0) 
            elem->x = elem->x - step_size; 
    }
    
    if (trajectory == 1){   // direita
        if (((elem->x + elem->x_side/2) + step_size) <= x_max)
            elem->x = elem->x + step_size;   
    }

    if (trajectory == 2){   // cima
        if (((elem->y - elem->y_side/2) - step_size) >= 0)
            elem->y = elem->y - step_size;
    }

    if (trajectory == 3){   //baixo
        if (((elem->y + elem->y_side/2) + step_size) <= y_max)
            elem->y = elem->y + step_size;
    }
}


void square_shot(square *element){
    bullet *shot;

	if (!element->face) 
        shot = pistol_shot(element->x - element->x_side/2, element->y, element->face, element->gun);										
	else if 
        (element->face == 1) shot = pistol_shot(element->x + element->x_side/2, element->y, element->face, element->gun);		

	if (shot) element->gun->shots = shot;
}

void square_jump(square *elem, unsigned short floor){

    if (!elem->is_jump && elem->y == floor - elem->y_side/2){
        elem->is_jump = 1;
    }

}

void square_punch(square *elem, square *opponent){
    
    if (elem->face == 0){
        printf("quase punch\n");
        if (opponent->x <= elem->x && opponent->x >= elem->x - PUNCH_RANGE &&
        opponent->y <= elem->y + elem->y_side/2 && opponent->y >= elem->y - elem->y_side/2){
            printf("dyenuing\n");
            if (!opponent->control->parry){
                opponent->hp--;
                update_health_bar(opponent->health_bar, opponent->hp);
                printf("%u\n", opponent->hp);
            }
        }
    } else if (elem->face == 1) { 
        printf("quase punch\n");
        if (opponent->x >= elem->x && opponent->x <= elem->x + PUNCH_RANGE &&
            opponent->y <= elem->y + elem->y_side/2 && opponent->y >= elem->y - elem->y_side/2) {
            printf("dyenuing\n");
            if (!opponent->control->parry){
                opponent->hp--;
                update_health_bar(opponent->health_bar, opponent->hp);
                printf("%u\n", opponent->hp); 
            }           
        }
    }
}

void square_kick(square *elem, square *opponent){

    if (elem->face == 0){
        printf("quase kick1\n");
        if (opponent->x <= elem->x && opponent->x >= elem->x - KICK_RANGE &&
        opponent->y <= elem->y + elem->y_side/2 && opponent->y >= elem->y - elem->y_side/2) {
            printf("tomou1\n");
            if (!opponent->control->parry){
                opponent->hp--;
                update_health_bar(opponent->health_bar, opponent->hp);
                printf("%u\n", opponent->hp);
            }
        }
    } else if (elem->face == 1) { 
        printf("quase kick\n");
        if (opponent->x >= elem->x && opponent->x <= elem->x + KICK_RANGE &&
            opponent->y <= elem->y + elem->y_side/2 && opponent->y >= elem->y - elem->y_side/2) {
            printf("tomou\n");
            if (!opponent->control->parry){
                opponent->hp--;
                update_health_bar(opponent->health_bar, opponent->hp);
                printf("%u\n", opponent->hp);
            }            
        }
    }
}

void square_crouch(square *elem){

    if (!elem->is_crouching){
        elem->is_crouching = 1;
        elem->y = elem->y + elem->y_side/4;
        elem->y_side -= elem->y_side/2;
    }

}

void square_stand(square *elem){

    if (elem->is_crouching){
        elem->is_crouching = 0;
        elem->y_side += elem->y_side;
        elem->y = elem->y - elem->y_side/4;
    }

}

void square_destroy(square* elem){

    pistol_destroy(elem->gun);																														
	joystick_destroy(elem->control);
    destroy_health_bar(elem->health_bar);
    free(elem);
}