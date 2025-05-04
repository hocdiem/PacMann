#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "defs.h"

using namespace std;


void renderBG(SDL_Texture *texture, int x, int y, int neww, int newh, SDL_Renderer* renderer){
    SDL_Rect hcn;
    hcn.x = x;
    hcn.y = y;

    int oriW, oriH;
    SDL_QueryTexture(texture, NULL, NULL, &oriW, &oriH);

    float scalew = (float)neww / oriW;
    float scaleh = (float)newh / oriH;

    float scale = (scalew > scaleh) ? scalew : scaleh;

    hcn.w = oriW * scale;
    hcn.h = oriH * scale;

    SDL_RenderCopy(renderer, texture, NULL, &hcn);
}

struct eatingPac {
    SDL_Texture* texture;
    vector<SDL_Rect> clips;
    int currentFrame = 0;

    void init(SDL_Texture* _texture, int frames, const int _clips[][4]){
        texture = _texture;
        SDL_Rect clip;
        for (int i = 0; i<frames; i++){
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];

            clips.push_back(clip);
        }
    }
    void tick(){
        currentFrame = (currentFrame + 1) % clips.size();
    }
    const SDL_Rect* getCurrentClip() const{
        return &(clips[currentFrame]);
    }
};

struct eyeroll {
    SDL_Texture* texture;
    vector<SDL_Rect> clips;
    int currentFrame = 0;
    void init(SDL_Texture* _texture, int frames, const int _clips[][4]){
        texture = _texture;
        SDL_Rect clip;
        for (int i = 0; i<4; i++){
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];

            clips.push_back(clip);
        }
    }
    void tick(){
        currentFrame = (currentFrame + 1) % clips.size();
    }
    const SDL_Rect* getCurrentClip() const{
        return &(clips[currentFrame]);
    }
};

struct Graphics {
    SDL_Renderer *renderer;
    SDL_Window *window;

    //graphic set up
    void logErrorAndExit(const char* msg, const char* error){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

    void init(){
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        //image
        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit("SDL_image error: ", SDL_GetError());

        window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) logErrorAndExit("create window", SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) logErrorAndExit("Create renderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        //mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
            logErrorAndExit("sdl_mixer could not initialise, sdl_mixer error: %s\n", Mix_GetError());
        }

        //text
        if (TTF_Init() == -1){
            logErrorAndExit("sdl_ttf could not initialise, sdl_ttf error: %s\n", TTF_GetError());
        }
    }

    void prepareScene(){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //SDL_RenderPresent(renderer);
    }

    void presentScene(){
        SDL_RenderPresent(renderer);
    }

    void quit(){
        Mix_Quit();
        IMG_Quit();
        TTF_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    SDL_Texture *loadTexture(const char *filename){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "loading %s", filename);
        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL){
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
        }
        return texture;
    }
    void renderTexture(SDL_Texture *texture, int x, int y){
        SDL_Rect hcn;
        hcn.x = x; hcn.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &hcn.w, &hcn.h);

        SDL_RenderCopy(renderer, texture, NULL, &hcn);
    }

    //music
    Mix_Music *loadMusic(const char* path){
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr){
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "could not load music, sdl_mixer error: %s", Mix_GetError());
        }

        return gMusic;
    }
    void play(Mix_Music *gMusic){
        if (gMusic == nullptr) return;
        if (Mix_PlayingMusic() == 0) Mix_PlayMusic(gMusic, -1);
        else if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
    }

    //make PacMan eat
    void renderP(int x, int y, const eatingPac& pac){
        const SDL_Rect* clip = pac.getCurrentClip();
        SDL_Rect renderq = {x, y, clip->w, clip->h};
        SDL_RenderCopy(renderer, pac.texture, clip, &renderq);
    }
    //ghost
    void renderG(int x, int y, const eyeroll& gho){
        const SDL_Rect* clip = gho.getCurrentClip();
        SDL_Rect render = {x, y, clip->w, clip->h};
        SDL_RenderCopy(renderer, gho.texture, clip, &render);
    }

    //draw map, set 0 for none, 1 for wall, 2 for dots
    void drawMap(int Map[MAP_H][MAP_W]){
        for (int i = 0; i<MAP_H; i++){
            for (int j = 0; j<MAP_W; j++){
                //SDL_Rect hcn = {i*tile, j*tile, tile, tile};
                if (Map[i][j] == 1){
                    SDL_Rect hcn = {j*tile, i*tile, tile, tile};
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    SDL_RenderFillRect(renderer, &hcn);
                }
                else if(Map[i][j] == 2){
                    SDL_Rect dot = {j*tile + tile/2 - DOT_SIZE/2, i*tile + tile/2 - DOT_SIZE/2, DOT_SIZE, DOT_SIZE};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &dot);
                }
            }
        }
    }
};

#endif // GRAPHICS_H_INCLUDED
