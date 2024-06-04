#ifndef __SQUARE__
#define __SQUARE__

#include "joystick.h"
#include "pistol.h"

#define SQUARE_STEP 10
typedef struct
{
    unsigned int side;
    unsigned char face;
    unsigned int x;
    unsigned int y;
    joystick* control;
    pistol* gun;

} square;

square* square_create(unsigned int side, unsigned char face, unsigned int x, unsigned int y, unsigned int x_max, unsigned int y_max);

void square_move(square* move, char steps, unsigned char trajectory, unsigned int x_max, unsigned int y_max);

void square_shot(square *element);

void square_destroy(square *elem);

#endif