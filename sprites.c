#include "sprites.h"
#include <stdio.h>
#include <stdlib.h>



static ALLEGRO_BITMAP** split_image(ALLEGRO_BITMAP* image, int num_frames, int largura_frame, int altura_frame){
    ALLEGRO_BITMAP** frames = (ALLEGRO_BITMAP**)malloc(num_frames * sizeof(ALLEGRO_BITMAP*));
    if (!frames){
        return NULL;
    }

    for (int i = 0; i < num_frames; i++){
        frames[i] = al_create_sub_bitmap(image, i * largura_frame, 0, largura_frame, altura_frame);
        if (!frames[i]) {
            // Libere todos os bitmaps criados até o ponto de falha
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(frames[j]);
            }
            free(frames);
            return NULL;
        }
    }
    return frames;
}


static Animation load_animation (const char* path, int num_frames, int largura_frame, int altura_frame, float frame_duration){
    Animation animation;
    animation.num_frames = num_frames;
    animation.frame_duration = frame_duration;

    ALLEGRO_BITMAP* image = al_load_bitmap(path);
    if (!image){
        animation.frames = NULL;
        animation.num_frames = 0;
        return animation;
    }

    animation.frames = split_image(image, num_frames, largura_frame, altura_frame);
    //al_destroy_bitmap(image);

    if (!animation.frames) {
        animation.num_frames = 0;
    }

    return animation;
}

Character* load_character (const char* base_folder, int walk_frames, int idle_frames, int crouching_frames, int punch_frames, int kick_frames, int jump_frames, int largura_frame, int altura_frame){
    Character* character = (Character*)malloc(sizeof(Character));
    if (!character){
        printf("erro ao alocar memoria para o personagem\n");
        return NULL;
    }

    char path[256];

    snprintf(path, sizeof(path), "%s/walking.png", base_folder);
    character->walking = load_animation(path, walk_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/jump.png", base_folder);
    character->jumping = load_animation(path, jump_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/punch.png", base_folder);
    character->punching = load_animation(path, punch_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/kick.png", base_folder);
    character->kicking = load_animation(path, kick_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/idle.png", base_folder);
    character->idle = load_animation(path, idle_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/crouch.png", base_folder);
    character->crouching = load_animation(path, crouching_frames, largura_frame, altura_frame, 0.1f);

    character->current_status = IDLE;
    character->current_frame = 0;
    character->time_to_next_frame = character->idle.frame_duration;

    return character;
}

void destroy_character(Character* character) {
    if (character) {
        for (int i = 0; i < character->walking.num_frames; i++) {
            al_destroy_bitmap(character->walking.frames[i]);
        }
        for (int i = 0; i < character->punching.num_frames; i++) {
            al_destroy_bitmap(character->punching.frames[i]);
        }
        for (int i = 0; i < character->jumping.num_frames; i++) {
            al_destroy_bitmap(character->jumping.frames[i]);
        }
        for (int i = 0; i < character->idle.num_frames; i++) {
            al_destroy_bitmap(character->idle.frames[i]);
        }
        free(character->walking.frames);
        free(character->punching.frames);
        free(character->jumping.frames);
        free(character->idle.frames);
        free(character);
    }
}

// Finalmente, desenhar animação
void draw_animation (Character* character, float x, float y, float delta_time){

    Animation* current_animation;

    switch (character->current_status){
        case WALKING:
            current_animation = &character->walking;
            break;
        case PUNCHING:
            current_animation = &character->punching;
            break;
        case JUMPING:
            current_animation = &character->jumping;
            break;
        case IDLE:
            current_animation = &character->idle;
            break;
        case KICKING:
            current_animation = &character->kicking;
            break;
        case CROUCH:
            current_animation = &character->crouching;
            break;
        default:
            current_animation = &character->idle;
            break;
    }

    character->time_to_next_frame -= delta_time;
    if (character->time_to_next_frame <= 0){
        character->current_frame = (character->current_frame + 1) % current_animation->num_frames;
        character->time_to_next_frame = current_animation->frame_duration;
    }
    printf("Current Status: %d\n", character->current_status);
    printf("Number of Frames: %d\n", current_animation->num_frames);
    printf("Frame Duration: %f\n", current_animation->frame_duration);
    printf("Current Frame Index: %d\n", character->current_frame);

    if (current_animation->frames[character->current_frame] == NULL) {
        printf("Error: Current frame bitmap is NULL!\n");
    } else {
        printf("Drawing bitmap at index %d\n", character->current_frame);
    }

    al_draw_bitmap(current_animation->frames[character->current_frame], x, y, 0);
    
}


void update_character_status(Character* character, joystick* control) {
    int previous_status = character->current_status;
    
    if (control->right) {
        character->current_status = WALKING;
    } else if (control->left) {
        // Supondo que há uma animação para mover para a esquerda
        character->current_status = WALKING;
    } else if (control->crouch){
        character->current_status = CROUCH;
    } else if (control->jump) {
        character->current_status = JUMPING;
    }
    else if (control->kick){
        character->current_status = KICKING;
    }
    else
        character->current_status = IDLE;

    // Somente atualize o frame e o tempo se o status mudou
    if (character->current_status != previous_status) {
        // Verifique o número de frames da nova animação
        Animation* new_animation;
        switch (character->current_status) {
            case WALKING:
                new_animation = &character->walking;
                break;
            case PUNCHING:
                new_animation = &character->punching;
                break;
            case JUMPING:
                new_animation = &character->jumping;
                break;
            case IDLE:
                new_animation = &character->idle;
                break;
            case KICKING:
                new_animation = &character->kicking;
                break;
            case CROUCH:
                new_animation = &character->crouching;
                break;
            default:
                new_animation = &character->idle;
                break;
        }

        // Certifique-se de que current_frame está dentro dos limites válidos
        if (character->current_frame >= new_animation->num_frames) {
            character->current_frame = 0;  // Reinicie o frame se estiver fora dos limites
        }

        character->time_to_next_frame = new_animation->frame_duration;
    }
}