#include "bot.h"
#include "joystick.h"


void update_bot_joystick(square *bot, square *player, Character *bot_character){
    
    bot->bot_action_timer -= 1;

    float distance = sqrt(pow(player->x - bot->x, 2) + pow(player->y - bot->y, 2));

    if (player->x > bot->x)
        bot->face = 1;
    else 
        bot->face = 0;

    if (!bot->is_jump && !bot->is_faling){
        bot->control->jump = 0;
    }

    if (bot->bot_action_timer <= 0){
        if (bot->is_crouching){
           square_stand(bot);
           bot->control->crouch = 0;
        }
        if (bot->control->parry){
            bot->control->parry = 0;
        }
        bot->is_punching = 0;
        bot->control->punch = 0;

        bot->control->kick = 0;
        bot->is_kicking = 0;
    }

    if (distance < 80.0f && bot->bot_action_timer <= 0){


        printf("entrou\n");
        int random_action = rand() % 9;
        
        printf("random: %d\n", random_action);
        switch (random_action) {
            case 0:
                joystick_punch(bot->control);
                break;
            case 1:
                joystick_kick(bot->control);
                break;
            case 2:
                joystick_jump(bot->control); 
                break;
            case 3:
                joystick_crouch(bot->control);
                break;
            case 4:
                bot->control->left = 0;
                bot->control->right = 0;
                joystick_parry(bot->control);
                break;
            case 5:
                joystick_punch(bot->control);
                joystick_jump(bot->control);
                break;
            case 6:
                joystick_punch(bot->control);
                joystick_crouch(bot->control);
                break;
            case 7:
                joystick_kick(bot->control);
                joystick_crouch(bot->control);
                break;
            case 8:
                joystick_kick(bot->control);
                joystick_jump(bot->control);
                break;
            default:
                break;
        }
        bot->bot_action_timer = 30;

        
    }
    else if (distance > 150.0f){
        if (bot->face == 1) {
            bot->control->left = 0;
            bot->control->right = 1;
        }  
        else {
            bot->control->right = 0;
            bot->control->left = 1;  
        }
    }
    
    bot->previous_direction = bot->face;
}
