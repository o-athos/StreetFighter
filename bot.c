#include "bot.h"


void update_bot_joystick(square *bot, square *player, Character *bot_character){
    
    bot->bot_action_timer -= 1;

    float distance = sqrt(pow(player->x - bot->x, 2) + pow(player->y - bot->y, 2));

    int direction = (player->x > bot->x) ? 1 : -1;

    

    if (distance < 100.0f && bot->bot_action_timer <= 0){

        printf("entrou\n");
        int random_action = rand() % 6;
        
        printf("random: %d\n", random_action);
        switch (random_action) {
            case 0:
                joystick_punch(bot->control);
                break;
            case 1:
                joystick_punch(bot->control);
                break;
            case 2:
                joystick_kick(bot->control);
                break;
            case 3:
                joystick_jump(bot->control); 
                break;
            case 4:
                joystick_crouch(bot->control);
                break;
            case 5:
                joystick_parry(bot->control);
                break;
            default:
                break;
        }
        bot->bot_action_timer = 20;

        
    }
    else{
        if (direction == 1) {
            joystick_right(bot->control);
        }  
        else {
            joystick_left(bot->control);  
        }
    }
    bot->previous_direction = direction;
}




void update_bot(square *bot, square *player, Character *bot_character, float delta_time){
    int previous_status = bot_character->current_status;
    int previous_direction = bot->previous_direction;

    update_bot_joystick(bot, player, bot_character);

    if (bot->is_punching){
        bot_character->current_status = PUNCHING;
    }
    else if (bot->is_kicking){
        bot_character->current_status = KICKING;
    }
    else if (bot->is_jump || bot->is_faling){
        bot_character->current_status = JUMPING;
    }
    else if (bot->control->right || bot->control->left){
        bot_character->current_status = WALKING;
    }
    else if (bot->is_crouching){
        bot_character->current_status = CROUCH;
    }
    else if (bot->control->parry){
        bot_character->current_status = PARRY_UP;
    }


    
    if (bot_character->current_status != previous_status ) {
        Animation* new_animation;
        printf("crash\n");
        switch (bot_character->current_status) {
            case WALKING:
                new_animation = &bot_character->walking;
                break;
            case PUNCHING:
                new_animation = &bot_character->punching;
                //joystick_punch(bot->control);
                break;
            case KICKING:
                new_animation = &bot_character->kicking;
                //joystick_kick(bot->control);
                break;
            case JUMPING:
                new_animation = &bot_character->jumping;
                //joystick_jump(bot->control);
                break;
            case CROUCH:
                new_animation = &bot_character->crouching;
                //joystick_crouch(bot->control);
                break;
            case PARRY_UP:
                new_animation = &bot_character->parry_up;
                //joystick_parry(bot->control);
                break;
            default:
                new_animation = &bot_character->idle;
                break;
        
        }

        if (bot_character->current_frame >= new_animation->num_frames) {
            bot_character->current_frame = 0;  // Reinicie o frame se estiver fora dos limites
        }

        bot_character->time_to_next_frame = new_animation->frame_duration;
    }
}
