#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"

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
    }

    void prepareScene(SDL_Texture *background){
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
    }

    void presentScene(){
        SDL_RenderPresent(renderer);
    }

    void quit(){
        Mix_Quit();
        IMG_Quit();

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
};

#endif // GRAPHICS_H_INCLUDED
