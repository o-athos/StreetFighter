#ifndef __JOYSTICK__
#define __JOYSTICK__

typedef struct {

    unsigned char left;
    unsigned char right;
    unsigned char up;
    unsigned char down;
    unsigned char fire;
    unsigned char jump;

} joystick;

joystick* joystick_create();

void joystick_destroy(joystick* elem);

void joystick_left(joystick* elem); 

void joystick_right(joystick* elem); 

void joystick_up(joystick* elem); 

void joystick_down(joystick* elem); 

void joystick_fire(joystick* elem);

void joystick_jump(joystick* elem);

#endif