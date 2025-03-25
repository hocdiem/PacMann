#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
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

    bool quit = false;
    SDL_Event ee;
    while (!quit){
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
        render()
        SDL_Delay(10);
    }
    if (gmusic != nullptr) Mix_FreeMusic(gmusic);

    //fixed do not touch
    graphic.quit();
    return 0;
}
