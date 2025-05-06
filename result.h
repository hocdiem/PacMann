#ifndef RESULT_H_INCLUDED
#define RESULT_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>

struct outro{
    SDL_Surface* continueSurface = nullptr;
    SDL_Texture* continueTexture = nullptr;
    TTF_Font* continueFont = nullptr;
    SDL_Surface* yesSurface = nullptr;
    SDL_Texture* yesTexture = nullptr;
    SDL_Surface* noSurface = nullptr;
    SDL_Texture* noTexture = nullptr;

    ~outro(){
        cleanup();
    }

    void cleanup(){
        if(continueSurface) SDL_FreeSurface(continueSurface);
        if(continueTexture) SDL_DestroyTexture(continueTexture);
        if(yesSurface) SDL_FreeSurface(yesSurface);
        if(yesTexture) SDL_DestroyTexture(yesTexture);
        if(noTexture) SDL_DestroyTexture(noTexture);
        if(noSurface) SDL_FreeSurface(noSurface);
        if(continueFont) TTF_CloseFont(continueFont);
    }
};

bool showResult(SDL_Renderer *renderer){
    outro showkq;
    showkq.continueFont = TTF_OpenFont("VHARIAL.TTF", 30);
    if (!showkq.continueFont) {
        SDL_Log("Failed to load fonts in result: %s", TTF_GetError());
        return false;
    }

    SDL_Color white = {255, 255, 255, 255};

    showkq.continueSurface = TTF_RenderText_Solid(showkq.continueFont, "CONTINUE?", white);
    showkq.yesSurface = TTF_RenderText_Solid(showkq.continueFont, "YES!", white);
    showkq.noSurface = TTF_RenderText_Solid(showkq.continueFont, "NO!", white);

    showkq.continueTexture = SDL_CreateTextureFromSurface(renderer, showkq.continueSurface);
    showkq.yesTexture = SDL_CreateTextureFromSurface(renderer, showkq.yesSurface);
    showkq.noTexture = SDL_CreateTextureFromSurface(renderer, showkq.noSurface);

    SDL_Rect continueRect = {
        (SCREEN_WIDTH - showkq.continueSurface->w) / 2,
        500,
        showkq.continueSurface->w,
        showkq.continueSurface->h
    };

    SDL_Rect yesRect = {
        (SCREEN_WIDTH - showkq.yesSurface->w) / 2 + 100,
        550,
        showkq.yesSurface->w, showkq.yesSurface->h
    };
    SDL_Rect noRect = {
        (SCREEN_WIDTH - showkq.yesSurface->w) / 2 - 100,
        550,
        showkq.noSurface->w, showkq.noSurface->h
    };
    SDL_Texture* skull = IMG_LoadTexture(renderer, "skull.png");
    SDL_Rect skullRect = {
        (SCREEN_WIDTH - 400) / 2,
        100,
        400,
        400
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

        if (skull) {
            SDL_RenderCopy(renderer, skull, NULL, &skullRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        SDL_RenderCopy(renderer, showkq.continueTexture, NULL, &continueRect);
        SDL_RenderCopy(renderer, showkq.yesTexture, NULL, &yesRect);
        SDL_RenderCopy(renderer, showkq.noTexture, NULL, &noRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}
#endif // RESULT_H_INCLUDED
