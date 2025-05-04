#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <cmath>
#include "defs.h"
#include "graphics.h"
#include "characters.h"
#include "intro.h"
#include "result.h"

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
    bool restartGame = true;

    while(restartGame){
        restartGame = false;

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
        eyeroll fire;
        SDL_Texture* ghostTextureR = graphic.loadTexture("mado-01.png");
        fire.init(ghostTextureR, GHOST_FRAMES, GHOST_CLIPS);
        eyeroll cam;
        SDL_Texture* ghostTextureO = graphic.loadTexture("macam-01.png");
        cam.init(ghostTextureO, GHOST_FRAMES, GHOST_CLIPS);
        eyeroll hong;
        SDL_Texture* ghostTextureP = graphic.loadTexture("mahong-01.png");
        hong.init(ghostTextureP, GHOST_FRAMES, GHOST_CLIPS);

        //ghost run
        ghost blue(80, 40);
        ghost red(720, 80);
        ghost orange(80, 760);
        ghost pink(720, 760);
        vector<ghost*> ghosts = {&red, &blue, &orange, &pink};
        vector<eyeroll*> ghostAnimations = {&fire, &xanh, &cam, &hong};

        vector<pair<int, int>> ghostPath;
        size_t ghostPathIndex = 0;

        //main render
        Pac run;
        bool quit = false;
        SDL_Event e;
        //intro
        bool quitGame = showIntro(graphic.renderer, gmusic);
        if (quitGame) {
            Mix_HaltMusic();
            if (gmusic != nullptr) Mix_FreeMusic(gmusic);
            graphic.quit();
            return 0;
        }

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

            //this is for ghost
            Uint32 currentTime = SDL_GetTicks64();

            for (auto& g : ghosts) {
                if (g->heuristic(run.x, run.y) < chase) {
                    SDL_Log("khoang cach nho hon chase");
                    g->pathCalculate = true;
                    if (currentTime > g->lastPathCalculation + g->pathCalculationDelay) {
                        //changed
                        int targetTileX = (run.x + Psize / 2) / tile;
                        int targetTileY = (run.y + Psize / 2) / tile;
                        g->path = g->findPath(targetTileX * tile, targetTileY * tile);
                        g->pathIndex = 0;
                        g->lastPathCalculation = currentTime;
                    }

                    if (!g->path.empty()) {
                        if (g->pathIndex < g->path.size()) {
                            int nextX = g->path[g->pathIndex].first;
                            int nextY = g->path[g->pathIndex].second;

                            int dx = nextX - g->x;
                            int dy = nextY - g->y;
                            int dist = sqrt(dx * dx + dy * dy);

                            if (dist < g->speed) {
                                // Close enough, snap to target and go to next
                                g->x = nextX;
                                g->y = nextY;
                                g->pathIndex++;
                            } else {
                                // Normalize and move toward target
                                g->x += g->speed * dx / dist;
                                g->y += g->speed * dy / dist;
                            }
                        }
                    } else g->Move(); // fallback random move
                } else {
                    g->pathCalculate = false;
                    g->Move();
                }
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
            graphic.renderG(red.x, red.y, fire);
            graphic.renderG(pink.x, pink.y, hong);

            graphic.presentScene();

            SDL_Delay(100);
        }
        quitGame = showResult(graphic.renderer);

        Mix_HaltMusic();
        if (gmusic != nullptr) Mix_FreeMusic(gmusic);

        //fixed do not touch
        graphic.quit();

        if (!quitGame) {
            restartGame = true;
        }
    }
    return 0;
 }
