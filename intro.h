#ifndef INTRO_H_INCLUDED
#define INTRO_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "graphics.h"

struct IntroAssets {
    SDL_Surface* titleSurface = nullptr;
    SDL_Texture* titleTexture = nullptr;
    SDL_Surface* instructionSurface = nullptr;
    SDL_Texture* instructionTexture = nullptr;
    SDL_Surface* startSurface = nullptr;
    SDL_Texture* startTexture = nullptr;
    SDL_Surface* musicSurface = nullptr;
    SDL_Texture* musicTexture = nullptr;
    SDL_Surface* musicButtonSurface = nullptr;
    SDL_Texture* musicButtonTexture = nullptr;
    TTF_Font* bigFont = nullptr;
    TTF_Font* smallFont = nullptr;

    ~IntroAssets() {
        cleanup();
    }

    void cleanup() {
        if (titleSurface) SDL_FreeSurface(titleSurface);
        if (titleTexture) SDL_DestroyTexture(titleTexture);
        if (instructionSurface) SDL_FreeSurface(instructionSurface);
        if (instructionTexture) SDL_DestroyTexture(instructionTexture);
        if (startSurface) SDL_FreeSurface(startSurface);
        if (startTexture) SDL_DestroyTexture(startTexture);
        if (musicSurface) SDL_FreeSurface(musicSurface);
        if (musicTexture) SDL_DestroyTexture(musicTexture);
        if (musicButtonSurface) SDL_FreeSurface(musicButtonSurface);
        if (musicButtonTexture) SDL_DestroyTexture(musicButtonTexture);
        if (bigFont) TTF_CloseFont(bigFont);
        if (smallFont) TTF_CloseFont(smallFont);
    }
};

bool showIntro(SDL_Renderer* renderer, Mix_Music* music) {
    IntroAssets assets;
    bool musicOn = true;
    bool quitIntro = false;

    // Load fonts
    assets.bigFont = TTF_OpenFont("MontserratAlternates-ExtraBold.otf", 50);
    assets.smallFont = TTF_OpenFont("VHARIAL.TTF", 24);
    if (!assets.bigFont || !assets.smallFont) {
        SDL_Log("Failed to load fonts: %s", TTF_GetError());
        return false;
    }

    // Colors
    SDL_Color yellow = {255, 255, 0, 255};  // Pac-Man yellow
    SDL_Color white = {255, 255, 255, 255};

    // Create text surfaces
    assets.titleSurface = TTF_RenderText_Solid(assets.bigFont, "PAC-MAN", yellow);
    assets.instructionSurface = TTF_RenderText_Solid(assets.smallFont, "Press ARROW KEYS to move", white);
    assets.startSurface = TTF_RenderText_Solid(assets.smallFont, "Press any key to start", white);
    assets.musicSurface = TTF_RenderText_Solid(assets.smallFont, "MUSIC", white);
    assets.musicButtonSurface = TTF_RenderText_Solid(assets.smallFont, "ON", white);

    if (!assets.titleSurface || !assets.instructionSurface || !assets.startSurface ||
        !assets.musicSurface || !assets.musicButtonSurface) {
        SDL_Log("Failed to create text surfaces");
        return false;
    }

    // Create textures from surfaces
    assets.titleTexture = SDL_CreateTextureFromSurface(renderer, assets.titleSurface);
    assets.instructionTexture = SDL_CreateTextureFromSurface(renderer, assets.instructionSurface);
    assets.startTexture = SDL_CreateTextureFromSurface(renderer, assets.startSurface);
    assets.musicTexture = SDL_CreateTextureFromSurface(renderer, assets.musicSurface);
    assets.musicButtonTexture = SDL_CreateTextureFromSurface(renderer, assets.musicButtonSurface);

    // Set up rectangles for positioning
    SDL_Rect titleRect = {
        (SCREEN_WIDTH - assets.titleSurface->w) / 2,
        150,
        assets.titleSurface->w,
        assets.titleSurface->h
    };

    SDL_Rect instructionRect = {
        (SCREEN_WIDTH - assets.instructionSurface->w) / 2,
        300,
        assets.instructionSurface->w,
        assets.instructionSurface->h
    };

    SDL_Rect startRect = {
        (SCREEN_WIDTH - assets.startSurface->w) / 2,
        400,
        assets.startSurface->w,
        assets.startSurface->h
    };

    SDL_Rect musicRect = {
        (SCREEN_WIDTH - assets.musicSurface->w) / 2,
        500,
        assets.musicSurface->w,
        assets.musicSurface->h
    };

    SDL_Rect musicButtonRect = {
        (SCREEN_WIDTH - assets.musicButtonSurface->w) / 2,
        550,
        assets.musicButtonSurface->w,
        assets.musicButtonSurface->h
    };

    // Main intro loop
    SDL_Event e;
    while (!quitIntro) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return true; // Quit game
            }

            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                // Check if click is on music button
                int x, y;
                SDL_GetMouseState(&x, &y);
                bool clickOnMusicButton = (e.type == SDL_MOUSEBUTTONDOWN) &&
                                         (x >= musicButtonRect.x && x <= musicButtonRect.x + musicButtonRect.w) &&
                                         (y >= musicButtonRect.y && y <= musicButtonRect.y + musicButtonRect.h);

                if (!clickOnMusicButton) {
                    quitIntro = true; // Start game
                } else {
                    // Toggle music
                    musicOn = !musicOn;
                    SDL_FreeSurface(assets.musicButtonSurface);
                    SDL_DestroyTexture(assets.musicButtonTexture);

                    assets.musicButtonSurface = TTF_RenderText_Solid(assets.smallFont, musicOn ? "ON" : "OFF", white);
                    assets.musicButtonTexture = SDL_CreateTextureFromSurface(renderer, assets.musicButtonSurface);

                    musicButtonRect.w = assets.musicButtonSurface->w;
                    musicButtonRect.h = assets.musicButtonSurface->h;
                    musicButtonRect.x = (SCREEN_WIDTH - musicButtonRect.w) / 2;

                    if (musicOn) {
                        Mix_ResumeMusic();
                    } else {
                        Mix_PauseMusic();
                    }
                }
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 55);
        SDL_RenderClear(renderer);

        SDL_Texture *bg = IMG_LoadTexture(renderer, "images.jpg");
        if (bg) {
            SDL_SetTextureBlendMode(bg, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(bg, 100);

            renderBG(bg, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, renderer);
            SDL_DestroyTexture(bg);
        } else {
            SDL_Log("Failed to load background: %s", IMG_GetError());
            SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
            SDL_RenderClear(renderer);
        }

        // Draw title and instructions
        SDL_RenderCopy(renderer, assets.titleTexture, NULL, &titleRect);
        SDL_RenderCopy(renderer, assets.instructionTexture, NULL, &instructionRect);
        SDL_RenderCopy(renderer, assets.startTexture, NULL, &startRect);
        SDL_RenderCopy(renderer, assets.musicTexture, NULL, &musicRect);
        SDL_RenderCopy(renderer, assets.musicButtonTexture, NULL, &musicButtonRect);


        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    return false; // Don't quit game
}

#endif // INTRO_H_INCLUDED
