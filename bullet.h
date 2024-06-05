#ifndef __BULLET__
#define __BULLET__

#define BULLET_MOVE 8

typedef struct{

    unsigned int x;
    unsigned int y;
    unsigned char trajectory;
    struct bullet* next;

} bullet;

bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next);		

void bullet_move(bullet *elements);			

void bullet_destroy(bullet *element);																											

#endif