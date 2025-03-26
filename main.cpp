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
    SDL_Event ee;
    while (!quit && !gameOver(run)){
        graphic.prepareScene();
        while (SDL_PollEvent(&ee)){
            if (ee.type == SDL_QUIT) quit = true;
        }
        //handle keyboard of pacman moving
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) Pacman.goUp();
        if (currentKeyStates[SDL_SCANCODE_DOWN]) Pacman.goDown();
        if (currentKeyStates[SDL_SCANCODE_LEFT]) Pacman.goLeft();
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) Pacman.goRight();
        Pacman.move();

        while(SDL_PollEvent(&ee) != 0){
            if (ee.type == SDL_QUIT) quit = true;
        }
        pac.tick();

        SDL_Delay(10);
    }
    if (gmusic != nullptr) Mix_FreeMusic(gmusic);

    //fixed do not touch
    graphic.quit();
    return 0;
}
