#ifndef SPRITES_H
#define SPRITES_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "square.h"



typedef enum{
    WALKING,
    PUNCHING,
    JUMPING,
    IDLE,
    KICKING,
    CROUCH,
    PARRY_UP,
    PARRY_DOWN,
    VICTORY,
    CROUCH_PUNCH,
} StatusCharacter;


// Estrutura para armazenar uma animação
typedef struct {
    ALLEGRO_BITMAP** frames;
    int num_frames;
    float frame_duration;
} Animation;

// Estrutura para armazenar todas as animações de um personagem
typedef struct {
    Animation walking;
    Animation idle;
    Animation punching;
    Animation jumping;
    Animation kicking;
    Animation crouching;
    Animation parry_up;
    Animation parry_down;
    Animation victory;
    Animation crouch_punch;
    StatusCharacter current_status;
    int current_frame;
    float time_to_next_frame;
} Character;


// Funções para carregar e destruir sprites
Character* load_character(const char* caminho_base, int walking_frames, int idle_frames, int crouching_frames, int punching_frames,
                            int kick_frames, int jumping_frames, int parry_up_frames, int parry_down_frames, int victory_frames, int crouch_punch_frames,
                            int largura_frame, int altura_frame);

// Destroi
void destroy_character(Character* character);

// Função para desenhar a animação
void draw_animation(Character* character, float x, float y, unsigned char face, float delta_time);

void update_character_status(Character* character, square* control);

#endif
