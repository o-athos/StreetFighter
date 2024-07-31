#ifndef BOT_H
#define BOT_H

#include <stdlib.h>
#include <stdio.h>
#include "square.h"
#include "sprites.h"
#include "math.h"

void update_bot_joystick(square *bot, square *player, Character *bot_character);

void update_bot(square *bot, square *player, Character *bot_character, float delta_time);


#endif