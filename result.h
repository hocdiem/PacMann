#ifndef RESULT_H_INCLUDED
#define RESULT_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>
#include <string>
#include "characters.h"

struct outro{
    bool won = false;
    SDL_Surface* continueSurface = nullptr;
    SDL_Texture* continueTexture = nullptr;
    TTF_Font* continueFont = nullptr;
    SDL_Surface* yesSurface = nullptr;
    SDL_Texture* yesTexture = nullptr;
    SDL_Surface* noSurface = nullptr;
    SDL_Texture* noTexture = nullptr;
    SDL_Surface* deadSurface = nullptr;
    SDL_Texture* deadTexture = nullptr;

    ~outro(){
        cleanup();
    }

    bool showResult(SDL_Renderer *renderer){
        continueFont = TTF_OpenFont("VHARIAL.TTF", 30);
        if (!continueFont) {
            SDL_Log("Failed to load fonts in result: %s", TTF_GetError());
            return false;
        }

        SDL_Color white = {255, 255, 255, 255};

        continueSurface = TTF_RenderText_Solid(continueFont, "CONTINUE?", white);
        yesSurface = TTF_RenderText_Solid(continueFont, "YES!", white);
        noSurface = TTF_RenderText_Solid(continueFont, "NO!", white);
        stringstream ss;
        ss << "YOU DIED " << DIED << " TIMES";
        string text = ss.str();
        deadSurface = TTF_RenderText_Solid(continueFont, text.c_str(), white);

        continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
        yesTexture = SDL_CreateTextureFromSurface(renderer, yesSurface);
        noTexture = SDL_CreateTextureFromSurface(renderer, noSurface);
        deadTexture = SDL_CreateTextureFromSurface(renderer, deadSurface);

        SDL_Rect continueRect = {
            (SCREEN_WIDTH - continueSurface->w) / 2,
            500,
            continueSurface->w,
            continueSurface->h
        };

        SDL_Rect yesRect = {
            (SCREEN_WIDTH - yesSurface->w) / 2 + 100,
            550,
            yesSurface->w, yesSurface->h
        };
        SDL_Rect noRect = {
            (SCREEN_WIDTH - yesSurface->w) / 2 - 100,
            550,
            noSurface->w, noSurface->h
        };
        SDL_Texture* skull = IMG_LoadTexture(renderer, "skull.png");
        SDL_Texture* win = IMG_LoadTexture(renderer, "gettyimages-1447471508-480x480.jpg");
        SDL_Rect gameResult = {
            (SCREEN_WIDTH - 400) / 2,
            100,
            400,
            400
        };
        SDL_Rect deathRect {
            (SCREEN_WIDTH - deadSurface->w) / 2,
            600,
            deadSurface->w, deadSurface->h
        };

        SDL_Event e;
        while (true) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    return true;
                }

                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (x >= yesRect.x && x <= yesRect.x + yesRect.w &&
                        y >= yesRect.y && y <= yesRect.y + yesRect.h) {
                        return false;
                    }
                    if (x >= noRect.x && x <= noRect.x + noRect.w &&
                        y >= noRect.y && y <= noRect.y + noRect.h) {
                        return true;
                    }
                }
            }

            // Render
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 55);
            SDL_RenderClear(renderer);

            if(won){
                if (win) SDL_RenderCopy(renderer, win, NULL, &gameResult);
                else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                if (skull) SDL_RenderCopy(renderer, skull, NULL, &gameResult);
                else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);
            SDL_RenderCopy(renderer, yesTexture, NULL, &yesRect);
            SDL_RenderCopy(renderer, noTexture, NULL, &noRect);
            SDL_RenderCopy(renderer, deadTexture, NULL, &deathRect);

            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }
    }

    void cleanup(){
        if(continueSurface) SDL_FreeSurface(continueSurface);
        if(continueTexture) SDL_DestroyTexture(continueTexture);
        if(yesSurface) SDL_FreeSurface(yesSurface);
        if(yesTexture) SDL_DestroyTexture(yesTexture);
        if(noTexture) SDL_DestroyTexture(noTexture);
        if(noSurface) SDL_FreeSurface(noSurface);
        if(continueFont) TTF_CloseFont(continueFont);
        if(deadSurface) SDL_FreeSurface(deadSurface);
        if(deadTexture) SDL_DestroyTexture(deadTexture);
    }
};
#endif // RESULT_H_INCLUDED
