#ifndef CHARACTERS_H_INCLUDED
#define CHARACTERS_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"


struct Pac {
    int x = 400, y = 440, speed = INIT_SPEED;
    Direction dir;

    void Move(int Map[MAP_H][MAP_W]){
        int dx = x, dy = y;
        switch (dir){
            case UP: dy -= speed; break;
            case DOWN: dy += speed; break;
            case LEFT: dx -= speed; break;
            case RIGHT: dx +=  speed; break;
            default: break;
        }
        //define pacman in a square
        const int Psize = tile - 2;
        //check for walls
        int left = dx / tile;
        int right = (dx + Psize - 1) / tile;
        int top = dy / tile;
        int bottom = (dy + Psize - 1) / tile;

        // Check if all corners are within bounds and not hitting walls
        bool canMove = left >= 0 && right  < MAP_W && top >= 0 && bottom < MAP_H &&
                       Map[top][left] != 1 && Map[top][right] != 1 && Map[bottom][left] != 1 && Map[bottom][right] != 1;

        if (canMove) {
            x = dx;
            y = dy;

            // Check for dot in center tile (any corner works)
            int centerTileX = (x + Psize / 2) / tile;
            int centerTileY = (y + Psize / 2) / tile;
            if (Map[centerTileY][centerTileX] == 2) {
                Map[centerTileY][centerTileX] = 0;
            }
        }

        /**int tileX = dx / tile;
        int tileY = dy / tile;
        if (tileX >= 0 && tileX < MAP_W && tileY >= 0 && tileY <MAP_H && Map[tileY][tileX] != 1){
            x = dx; y = dy;

            if (Map[y][x] == 2){
                Map[y][x] = 0;
            }
        }**/
    }

    void setDirection(Direction neww){
        dir = neww;
    }
};

struct ghost {

};


bool gameOver(const Pac& pac){
    return pac.x < 0 || pac.x >= SCREEN_WIDTH ||
           pac.y < 0 || pac.y >= SCREEN_HEIGHT;
}

#endif // CHARACTERS_H_INCLUDED
