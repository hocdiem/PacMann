#ifndef CHARACTERS_H_INCLUDED
#define CHARACTERS_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
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

    }

    void setDirection(Direction neww){
        dir = neww;
    }
};

struct ghost {
    int x, y;
    int speed = GHOST_SPEED;

    ghost(int x_, int y_) {
        x = x_;
        y = y_;
    }

    void Move(int Map[MAP_H][MAP_W]){
        int direction = rand() % 4;
        int dx = x, dy = y;

        switch (direction){
            case 0: dy -= speed; break;
            case 1: dy += speed; break;
            case 2: dx -= speed; break;
            case 3: dx +=  speed; break;
        }

        //define pacman in a square
        const int Gsize = 35;
        //check for walls
        int left = dx / tile;
        int right = (dx + Gsize - 1) / tile;
        int top = dy / tile;
        int bottom = (dy + Gsize - 1) / tile;

        // Check if all corners are within bounds and not hitting walls
        bool canMove = left >= 0 && right  < MAP_W && top >= 0 && bottom < MAP_H &&
                       Map[top][left] != 1 && Map[top][right] != 1 && Map[bottom][left] != 1 && Map[bottom][right] != 1;

        if (canMove) {
            x = dx;
            y = dy;

            // Check for dot in center tile (any corner works)
            int centerTileX = (x + Gsize / 2) / tile;
            int centerTileY = (y + Gsize / 2) / tile;
        }
    }
};


bool gameOver(const Pac& pac){
    return pac.x < 0 || pac.x >= SCREEN_WIDTH ||
           pac.y < 0 || pac.y >= SCREEN_HEIGHT;
}

#endif // CHARACTERS_H_INCLUDED
