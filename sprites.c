#include "sprites.h"
#include <stdio.h>
#include <stdlib.h>



static ALLEGRO_BITMAP** split_image(ALLEGRO_BITMAP* image, int num_frames, int largura_frame, int altura_frame){
    ALLEGRO_BITMAP** frames = (ALLEGRO_BITMAP**)malloc(num_frames * sizeof(ALLEGRO_BITMAP*));
    if (!frames){
        return NULL;
    }

    for (int i = 0; i < num_frames; i++)
        frames[i] = al_create_sub_bitmap(image, i * largura_frame, 0, largura_frame, altura_frame);

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
    al_destroy_bitmap(image);

    if (!animation.frames) {
        animation.num_frames = 0;
    }

    return animation;
}

Character* load_character (const char* base_folder, int walk_frames, int punch_frames, int kick_frames, int jump_frames, int largura_frame, int altura_frame){
    Character* character = (Character*)malloc(sizeof(Character));
    if (!character){
        printf("erro ao alocar memoria para o personagem\n");
        return NULL;
    }

    char path[256];

    snprintf(path, sizeof(path), "%s/walk.png", base_folder);
    character->walking = load_animation(path, walk_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/jump.png", base_folder);
    character->jumping = load_animation(path, jump_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/punch.png", base_folder);
    character->punching = load_animation(path, punch_frames, largura_frame, altura_frame, 0.1f);

    snprintf(path, sizeof(path), "%s/kick.png", base_folder);
    character->kicking = load_animation(path, kick_frames, largura_frame, altura_frame, 0.1f);


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
        free(character->walking.frames);
        free(character->punching.frames);
        free(character->jumping.frames);
        free(character);
    }
}

// Finalmente, desenhar animação
void draw_animation (Animation* animation, int frame, unsigned char x, unsigned char y){
    if (frame < animation->num_frames){
        al_draw_bitmap(animation->frames[frame], x, y, 0);
    }
}