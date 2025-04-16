#ifndef CHARACTERS_H_INCLUDED
#define CHARACTERS_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
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
    int direction;

    ghost(int x_, int y_) {
        x = x_;
        y = y_;
    }

    // Check if all corners are within bounds and not hitting walls
    void Move(){
        //calculate a random first move
        static bool start = false;
        if (!start){
            srand(time(0));
            start = true;
        }

        int dx = x, dy = y;
        switch (direction){
            case 0: dy -= speed; break;//up
            case 1: dx -= speed; break;//left
            case 2: dy += speed; break;//down
            case 3: dx += speed; break;//right
        }

        //define ghost in a square
        const int Gsize = tile - 6;
        //check for walls
        int left = dx / tile;
        int right = (dx + Gsize - 1) / tile;
        int top = dy / tile;
        int bottom = (dy + Gsize - 1) / tile;

        bool canMove = true;
        if (left < 0 || right >= MAP_W || top < 0 || bottom >= MAP_H) canMove = false;
        else {
            for (int _y = top; _y <= bottom && canMove; _y++){
                for (int _x = left; _x <= right && canMove; _x++){
                    if (MAP[_y][_x] == 1) canMove = false;
                }
            }
        }

        //bool crossroad = (x%tile == 0 && y%tile == 0);
        if (!canMove){
            vector<bool> availableDir(4, false);

            availableDir[0] = y > speed && MAP[(y - speed) / tile][x/tile] != 1;
            availableDir[1] = x > speed && MAP[y/tile][(x - speed) / tile] != 1;
            availableDir[2] = y + speed + Gsize - 1 < MAP_H * tile && MAP[(y + speed + Gsize - 1) / tile][x/tile] != 1;
            availableDir[3] = x + speed + Gsize - 1 < MAP_W * tile && MAP[y/tile][(x + speed + Gsize - 1) / tile] != 1;

            int reverseDir = (direction + 2) % 4;
            availableDir[reverseDir] = false;

            vector<int> possibleDir;
            for (int i = 0; i<4; i++){
                if (availableDir[i]) possibleDir.push_back(i);
            }
            if (!possibleDir.empty()){
                direction = possibleDir[rand() % possibleDir.size()];
            } else direction = reverseDir;
        }
        else {
            x = dx;
            y = dy;
        }
    }
};


bool gameOver(const Pac& pac){
    return pac.x < 0 || pac.x >= SCREEN_WIDTH ||
           pac.y < 0 || pac.y >= SCREEN_HEIGHT;
}

#endif // CHARACTERS_H_INCLUDED
