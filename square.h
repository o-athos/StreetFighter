#ifndef __SQUARE__
#define __SQUARE__

#include "joystick.h"
#include "pistol.h"

#define SQUARE_STEP 10
typedef struct
{
    unsigned char side;
    unsigned char face;
    unsigned short x;
    unsigned short y;
    joystick* control;
    pistol* gun;

} square;

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short x_max, unsigned int y_max);

void square_move(square* move, char steps, unsigned char trajectory, unsigned short x_max, unsigned short y_max);

void square_shot(square *element);

void square_destroy(square *elem);

#endif