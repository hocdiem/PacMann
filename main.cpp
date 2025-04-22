#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "defs.h"
#include "graphics.h"
#include "characters.h"
#include "intro.h"

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
    srand(time(0));

    //music
    Mix_Music *gmusic = graphic.loadMusic("Ticktack-ILLIT-16507480.mp3");
    graphic.play(gmusic);

    //pacman show up
    eatingPac pac;
    SDL_Texture* pacTexture = graphic.loadTexture(PACMAN_FILE);
    pac.init(pacTexture, PAC_FRAMES, PACMAN_CLIPS);
    //ghost show up
    eyeroll xanh;
    SDL_Texture* ghostTextureB = graphic.loadTexture("maxanh-01.png");
    xanh.init(ghostTextureB, GHOST_FRAMES, GHOST_CLIPS);
    eyeroll ddo;
    SDL_Texture* ghostTextureR = graphic.loadTexture("mado-01.png");
    ddo.init(ghostTextureR, GHOST_FRAMES, GHOST_CLIPS);
    eyeroll cam;
    SDL_Texture* ghostTextureO = graphic.loadTexture("macam-01.png");
    cam.init(ghostTextureO, GHOST_FRAMES, GHOST_CLIPS);
    eyeroll hong;
    SDL_Texture* ghostTextureP = graphic.loadTexture("mahong-01.png");
    hong.init(ghostTextureP, GHOST_FRAMES, GHOST_CLIPS);

    //ghost run
    ghost blue(80, 40);
    ghost red(720, 40);
    ghost orange(80, 760);
    ghost pink(720, 760);
    vector<ghost*> ghosts = {&blue, &red, &orange, &pink};
    vector<eyeroll*> ghostAnimations = {&xanh, &ddo, &cam, &hong};

    showIntro(graphic.renderer);


    //main render
    Pac run;
    bool quit = false;
    SDL_Event e;
    bool introRunning = true;
    while (introRunning) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            introRunning = false;
    }
    SDL_Delay(10);
}

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
        for (auto& g : ghosts) {
            g->Move();
        }

        for (const auto& g : ghosts) {
            if (checkCollision(run.x, run.y, Psize, g->x, g->y, Gsize)) {
                SDL_Log("YOU DIED!");
                quit = true;
                break;
            }
        }

        //set frames
        pac.tick();
        for (auto& anim : ghostAnimations) anim->tick();
        //draw map
        graphic.drawMap(MAP);

        graphic.renderP(run.x, run.y, pac);
        graphic.renderG(blue.x, blue.y, xanh);
        graphic.renderG(orange.x, orange.y, cam);
        graphic.renderG(red.x, red.y, ddo);
        graphic.renderG(pink.x, pink.y, hong);

        graphic.presentScene();

        SDL_Delay(100);
    }
    Mix_HaltMusic();
    if (gmusic != nullptr) Mix_FreeMusic(gmusic);

    //fixed do not touch
    graphic.quit();
    return 0;
 }
