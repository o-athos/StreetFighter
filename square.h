#ifndef __SQUARE__
#define __SQUARE__

#include "joystick.h"
#include "pistol.h"
#include "health_bar.h"

#define SQUARE_STEP 7
#define PUNCH_RANGE 75
#define KICK_RANGE 85
#define PUNCH_COOLDOWN 10
#define KICK_COOLDOWN 15
typedef struct
{
    unsigned char side;
    unsigned char face;
    unsigned short x;
    unsigned short y;
    unsigned char hp;
    int is_jump;
    int is_faling;
    unsigned char punch_timer;
    unsigned char kick_timer;
    joystick* control;
    pistol* gun;
    HealthBar *health_bar;

} square;

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short x_max, unsigned int y_max);

void square_move(square* move, char steps, unsigned char trajectory, unsigned short x_max, unsigned short y_max);

void square_shot(square *element);

void square_jump(square *elem, unsigned short floor);

void square_punch(square *elem, square *opponent);

void square_kick(square *elem, square *opponent);

void square_destroy(square *elem);

#endif