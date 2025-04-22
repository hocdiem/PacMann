#ifndef INTRO_H_INCLUDED
#define INTRO_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

void showIntro(SDL_Renderer* renderer) {
    // Load a font (replace with your font file)
    TTF_Font* font = TTF_OpenFont("MontserratAlternates-ExtraBold.otf", 36);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return;
    }

    // Colors
    SDL_Color yellow = {255, 255, 0, 255};  // Pac-Man yellow
    SDL_Color white = {255, 255, 255, 255};
    //SDL_Color black = {0, 0, 0, 255};

    // Clear screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render title "PAC-MAN"
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "PAC-MAN", yellow);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = {
        (840 - titleSurface->w) / 2,  // Centered X
        200,                         // Y position
        titleSurface->w,
        titleSurface->h
    };
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    // Render instructions
    TTF_Font* smallFont = TTF_OpenFont("VHARIAL.TTF", 24);
    if (!smallFont) {
        SDL_Log("Failed to load small font: %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }
    SDL_Surface* instructionSurface = TTF_RenderText_Solid(smallFont, "Press and hold ARROW KEYS to move", white);
    SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
    SDL_Rect instructionRect = {
        (840 - instructionSurface->w) / 2,
        300,
        instructionSurface->w,
        instructionSurface->h
    };
    SDL_RenderCopy(renderer, instructionTexture, NULL, &instructionRect);

    // Render "Press any key to start"
    SDL_Surface* startSurface = TTF_RenderText_Solid(smallFont, "Press any key to start", white);
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_Rect startRect = {
        (840 - startSurface->w) / 2,
        400,
        startSurface->w,
        startSurface->h
    };
    SDL_RenderCopy(renderer, startTexture, NULL, &startRect);

    // Display everything
    SDL_RenderPresent(renderer);

    // Clean up
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(instructionSurface);
    SDL_DestroyTexture(instructionTexture);
    SDL_FreeSurface(startSurface);
    SDL_DestroyTexture(startTexture);
    TTF_CloseFont(font);
    TTF_CloseFont(smallFont);
    TTF_Quit();
}

#endif // INTRO_H_INCLUDED
