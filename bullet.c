#include "bullet.h"
#include <stdlib.h>


bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next){
    
    if ((trajectory < 0) || (trajectory > 1)) return NULL;  // ou 0 ou 1

    bullet* new_bullet = (bullet*)malloc(sizeof(bullet));
    new_bullet->x = x;
    new_bullet->y = y;
    new_bullet->trajectory = trajectory;
    new_bullet->next = (struct bullet*) next;

    return new_bullet;
}	

void bullet_move(bullet *elements){

    for (bullet *index = elements; index != NULL; index = (bullet*) index->next){							
		if (!index->trajectory) 
            index->x = index->x - BULLET_MOVE;											
		else 
            index->x = index->x + BULLET_MOVE;																
    }
}

void bullet_destroy(bullet *elem){
    free(elem);
}