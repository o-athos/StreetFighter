#include "pistol.h"
#include <stdlib.h>

pistol* pistol_create(){

    pistol* new_gun = (pistol*)malloc(sizeof(pistol));
    if (!new_gun) return NULL;

    new_gun->timer = 0;
    new_gun->shots = NULL;

    return new_gun;
}

bullet* pistol_shot(unsigned short x, unsigned short y, unsigned char trajectory, pistol *gun){
    
    bullet* new_shot = bullet_create(x, y, trajectory, gun->shots);
    if (!new_shot) return NULL;

    return new_shot;
}

void pistol_destroy(pistol* elem){
    
    bullet* sentinel;
    for (bullet *index = elem->shots; index != NULL; index = sentinel){							
		sentinel = (bullet*) index->next;																
		bullet_destroy(index);	
    }																		
	free(elem);
}