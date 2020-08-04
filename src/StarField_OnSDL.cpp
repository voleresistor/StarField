#include "StarField.h"        // Star struct and other includes

// Initialize SDL and create our window and generator
bool StarField::init() {
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't initialize SDL! Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("Star Field", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
        std::cout << "Error creating SDL window! Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL) {
        std::cout << "Error creating SDL renderer! Error: " << SDL_GetError() << "\n";
        return false;
    }
    else {
        // Black background
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0x0);
    }

    return true;
}

// Catch SDL events to handle a quit signal
void StarField::onEvent(SDL_Event* e) {
    // Clicking x
    if(e->type == SDL_QUIT) {
        quit = true;
    }

    if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_SPACE:
            if(paused) {
                paused = false;
            }
            else {
                paused = true;
            }
            break;

            case SDLK_c:
            if(!onConfig()) {
                std::cout << "Failed to init config window.\n";
            }
            break;
        }
    }
}

// Free up resources and close down the SDL systems
void StarField::close() {
    // Free up window and renderer
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    // Close out SDL subsystems
    SDL_Quit();
}

/* Config functions not yet fully realized. Currently opening   *
 * the config window causes the program to continue rendering   *
 * stars but become unresponsize to new input.                  */
// Free up SDL resources from config window
void StarField::closeConfig() {
    // Free up window and renderer
    inConfig = false;
    SDL_DestroyRenderer(cRenderer);
    SDL_DestroyWindow(cWindow);
    cRenderer = NULL;
    cWindow = NULL;
}

bool StarField::onConfig() {
    cRenderer = NULL;
    cWindow = NULL;

    // Create window
    cWindow = SDL_CreateWindow("Star Field Config", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if(cWindow == NULL) {
        std::cout << "Error creating SDL window! Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Create renderer
    cRenderer = SDL_CreateRenderer(cWindow, -1, SDL_RENDERER_ACCELERATED);
    if(cRenderer == NULL) {
        std::cout << "Error creating SDL renderer! Error: " << SDL_GetError() << "\n";
        return false;
    }
    else {
        // Gray? background
        SDL_SetRenderDrawColor(gRenderer, 0x7F, 0x7F, 0x7F, 0x0);
    }

    return true;
}