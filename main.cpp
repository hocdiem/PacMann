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
    Pac run;
    bool quit = false;
    SDL_Event e;

    //int lastDir = -1;
    while (!quit && !gameOver(run)){
        graphic.prepareScene();
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) quit = true;
        }

        //handle keyboard of pacman moving

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            run.setDirection(UP);
        }
        else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            run.setDirection(DOWN);
        }
        else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            run.setDirection(LEFT);
        }
        else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            run.setDirection(RIGHT);
        } else run.setDirection(NONE);

        run.Move(MAP);

        pac.tick();
        //graphic.prepareScene();
        //draw map
        graphic.drawMap(MAP);

        graphic.render(run.x, run.y, pac);
        graphic.presentScene();

        SDL_Delay(100);
    }
    if (gmusic != nullptr) Mix_FreeMusic(gmusic);

    //fixed do not touch
    graphic.quit();
    return 0;
}
