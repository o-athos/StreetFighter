#ifndef __PISTOL__
#define __PISTOL__

#include "bullet.h"

#define PISTOL_FIRE_RATE 10

typedef struct {

    unsigned char timer;
    bullet* shots;

} pistol;

pistol* pistol_create();

bullet* pistol_shot(unsigned short x, unsigned short y, unsigned char trajectory, pistol *gun);

void pistol_destroy(pistol* elem);

#endif