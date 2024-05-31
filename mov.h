#ifndef __MOV__
#define __MOV__

#include "square.h"

unsigned char collision_2D(square *element_first, square *element_second);

void update_position(square *player_1, square *player_2);

#endif