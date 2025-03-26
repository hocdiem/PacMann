#ifndef CHARACTERS_H_INCLUDED
#define CHARACTERS_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"

struct Run {
    int x, y;
    int dx = 0, dy = 0;
    int speed = INIT_SPEED;
    void move(){
        x += dx; y += dy;
    }
    void goUp(){
        dy -= speed; dx = 0;
    }
    void goDown(){
        dy += speed; dx = 0;
    }
    void goLeft() {
        dy = 0; dx -= speed;
    }
    void goRight() {
        dy = 0; dx += speed;
    }
    void dontChangeSpeed(){
        speed = 0;
    }
    void backToSpeed(){
        speed = INIT_SPEED;
    }
};

struct ghost {

};


bool gameOver(const Run& pac){
    return pac.x < 0 || pac.x >= SCREEN_WIDTH ||
           pac.y < 0 || pac.y >= SCREEN_HEIGHT;
}

#endif // CHARACTERS_H_INCLUDED
