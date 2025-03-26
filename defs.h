#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

//simple set up
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 840;
const int tile = 40;
const int MAP_W = SCREEN_WIDTH / tile;
const int MAP_H = SCREEN_HEIGHT / tile;
const int INIT_SPEED = 5;
const char* TITLE = "PacMan";

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

#endif // DEFS_H_INCLUDED
