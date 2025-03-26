#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "defs.h"
#include "graphics.h"
#include "characters.h"

using namespace std;

void waitUntilKeyPressed(){
    SDL_Event e;
    while (true){
        if (SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(10);
    }
}

int main(int argc, char *argv[])
{
    //fixed do not touch
    Graphics graphic;
    graphic.init();

    //music
    Mix_Music *gmusic = graphic.loadMusic("Ticktack-ILLIT-16507480.mp3");
    graphic.play(gmusic);

    //pacman show up
    eatingPac pac;
    SDL_Texture* pacTexture = graphic.loadTexture(PACMAN_FILE);
    pac.init(pacTexture, PAC_FRAMES, PACMAN_CLIPS);

    //main render
    Run run;
    run.x = SCREEN_WIDTH /2;
    run.y = SCREEN_HEIGHT /2;

    bool quit = false;
    SDL_Event e, preE;

    int lastDir = -1;
    while (!quit && !gameOver(run)){
        graphic.prepareScene();
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) quit = true;
        }
        int currentDir = -1;

        //handle keyboard of pacman moving
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            run.backToSpeed();
            currentDir = SDL_SCANCODE_UP;
            if(lastDir != SDL_SCANCODE_UP){
                run.goUp();
                run.dontChangeSpeed();
            }
        }
        else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            run.backToSpeed();
            currentDir = SDL_SCANCODE_DOWN;
            if (lastDir != SDL_SCANCODE_DOWN) {
                run.goDown();
                run.dontChangeSpeed();
            }
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            run.backToSpeed();
            currentDir = SDL_SCANCODE_LEFT;
            if (lastDir != SDL_SCANCODE_LEFT){
                run.goLeft();
                run.dontChangeSpeed();
            }
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            run.backToSpeed();
            currentDir = SDL_SCANCODE_RIGHT;
            if(lastDir != SDL_SCANCODE_RIGHT){
                run.goRight();
                run.dontChangeSpeed();
            }
        }
        if(currentDir != -1) lastDir = currentDir;
        preE = e;

        run.move();

        while(SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT) quit = true;
        }
        pac.tick();
        graphic.prepareScene();
        graphic.render(run.x, run.y, pac);
        graphic.presentScene();

        SDL_Delay(100);
    }
    if (gmusic != nullptr) Mix_FreeMusic(gmusic);

    //fixed do not touch
    graphic.quit();
    return 0;
}
