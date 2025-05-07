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
#include <queue>
#include <unordered_map>

bool checkCollision(int x1, int y1, int s1,
                    int x2, int y2, int s2) {
    return !(x1 + s1 <= x2 || x2 + s2 <= x1 ||
             y1 + s1 <= y2 || y2 + s2 <= y1);
}

struct Node {
    int x, y;
    int gCost, hCost;
    Node* parent;

    int fCost() const { return gCost + hCost; }
};
struct CompareNodes {
    bool operator()(Node* a, Node* b) {
        return a->fCost() > b->fCost();
    }
};

struct Pac {
    int x = 0, y = 360, speed = INIT_SPEED;
    int dot = 0;
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
                dot++;
                SDL_Log("%d", dot);
            }
            if (dot == 182){}
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
    int stuckCounter = 0;
    int lastTargetX = -1, lastTargetY = -1;
    vector<pair<int, int>> path;
    size_t pathIndex = 0;
    bool inChaseRange = false;

    //these lines
    Uint32 lastPathCalculation = 0;
    const Uint32 pathCalculationDelay = 200;
    bool pathCalculate = false;

    ghost(int x_, int y_) : x(x_), y(y_) {
        // Red ghost gets different initial direction
        if (x_ == 720 && y_ == 80) {
            direction = LEFT;
        } else {
            direction = rand() % 4;
        }
    }

    // Check if all corners are within bounds and not hitting walls
    bool isWall(int _x, int _y) {
        if (_x < 0 || _y < 0 || _x >= MAP_W || _y >= MAP_H) return true;
        return MAP[_y][_x] == 1;
    }

    int heuristic(int x1, int y1) {
        int tileX = x / tile;
        int tileY = y / tile;
        x1 /= tile;
        y1 /= tile;
        return abs(tileX - x1) + abs(tileY - y1);
    }

    // A* pathfinding
    vector<pair<int, int>> findPath(int endX, int endY) {
        int startX = static_cast<int>(x) / tile, startY = static_cast<int>(y) / tile;
        endX /= tile, endY /= tile;

        if (endX < 0 || endY < 0 || endX >= MAP_W || endY >= MAP_H || isWall(endX, endY)) {
            return {};
        }

        if (isWall(endX, endY)) return {};

        priority_queue<Node*, vector<Node*>, CompareNodes> openSet;
        bool closedSet[MAP_H][MAP_W] = {false};

        Node* startNode = new Node{startX, startY, 0, heuristic(endX, endY), nullptr};
        openSet.push(startNode);

        while (!openSet.empty()) {
            Node* current = openSet.top();
            openSet.pop();

            //found path and rewind back
            if (current->x == endX && current->y == endY) {
                vector<pair<int, int>> path;
                Node* temp = current;
                while (temp != nullptr) {
                    int px = temp->x * tile + (tile - Gsize) / 2;
                    int py = temp->y * tile + (tile - Gsize) / 2;
                    path.push_back({px, py});

                    temp = temp->parent;
                }
                reverse(path.begin(), path.end());
                while (!openSet.empty()) {
                    delete openSet.top();
                    openSet.pop();
                }
                return path;
            }

            closedSet[current->y][current->x] = true;

            const int dx[] = {0, 1, 0, -1};
            const int dy[] = {-1, 0, 1, 0};

            for (int i = 0; i < 4; ++i) {
                int newX = current->x + dx[i];
                int newY = current->y + dy[i];

                if (isWall(newX, newY) || closedSet[newY][newX]) continue;

                Node* neighbor = new Node{
                    newX, newY,
                    current->gCost + 1,
                    heuristic(endX, endY),
                    current
                };
                openSet.push(neighbor);
            }
        }

        return {};
    }

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
