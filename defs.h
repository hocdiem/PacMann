#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

//simple set up
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 840;
const int tile = 40;
const int DOT_SIZE = 5;
const int MAP_W = (SCREEN_WIDTH - 60) / tile;
const int MAP_H = SCREEN_HEIGHT / tile;
const int INIT_SPEED = 10;
const int GHOST_SPEED = 8;
const char* TITLE = "PacMan";
enum Direction {UP, DOWN, LEFT, RIGHT, NONE};

//set up frames of pac man
const char* PACMAN_FILE = "pac-01.png";
const int PACMAN_CLIPS[][4] = {
    {0, 0, 35, 35},
    {35, 0, 35, 35},
    {70, 0, 35, 35},
    {105, 0, 35, 35},
    {140, 0, 35, 35},
    {175, 0, 35, 35},
    {210, 0, 35, 35},
    {245, 0, 35, 35},
    };
const int PAC_FRAMES = sizeof(PACMAN_CLIPS) / sizeof(int)/4;
//define pacman in a square
const int Psize = tile - 2;

//set up frames for the ghosts
const int GHOST_CLIPS[][4] = {{0, 0, 35, 35},{0, 0, 35, 35}, {35, 0, 35, 35}, {35, 0, 35, 35}};
const int GHOST_FRAMES = 2;
//define ghost in a square
const int Gsize = tile - 6;

//this is the array for drawing map, 0 for none, 1 for wall, 2 for dots
int MAP[MAP_H][MAP_W] = {
//   1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},//1
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},//2
    {0, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 0},//3
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},//4
    {0, 1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1, 0},//5
    {0, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1, 0},//6
    {0, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 0},//7
    {0, 0, 0, 0, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 0, 0, 0, 0},//8
    {1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1},//9
    {0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0},//10
    {1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1},//11
    {0, 0, 0, 0, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 0, 0, 0, 0},//12
    {0, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 0},//13
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},//14
    {0, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 0},//15
    {0, 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 0},//16
    {0, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 0},//17
    {0, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1, 0},//18
    {0, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 0},//19
    {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},//20
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}//21
    };
//start chasing at this radius
const int chase = 8;

#endif // DEFS_H_INCLUDED
