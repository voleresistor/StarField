#include "StarField.h"        // Star struct and other includes

// This class owns all of our vars and functions
StarField::StarField() {
    // SDL window and renderer
    gWindow = NULL;
    gRenderer = NULL;

    // Loop tracker
    quit = false;
    inConfig = false;
    paused = false;
}

int StarField::OnExecute() {
    // initialize random numbers and SDL
    if(!init()) {
        std::cout << "Init failed.\n";
        return RETURN_ERROR;
    }

    // SDL events
    SDL_Event e;

    // initialize random library to use Mersenne pseudo-random generators
    std::mt19937 generator(time(NULL));
    std::uniform_real_distribution<double> randX(-SCREEN_WIDTH, SCREEN_WIDTH);
    std::uniform_real_distribution<double> randY(-SCREEN_HEIGHT, SCREEN_HEIGHT);
    std::uniform_real_distribution<double> randZ(2.0, 5.5);
    std::uniform_real_distribution<double> randR(0.3, 0.6);
    std::uniform_real_distribution<double> randC(1.0, 10.0);

    /* Create a new star array based on a calculated number of stars    *
     * and ensure that it's populated with a Star() object that we can  *
     * use to render the stars.                                         */
    int starCount = SCREEN_HEIGHT * STAR_FILL_RATE;
    //int starCount = 1;
    Star stars[starCount];
    for(int i = 0; i < starCount; i++) {
        stars[i] = Star(randX(generator), randY(generator), randZ(generator),
            randR(generator), randC(generator), i);
    }

    while(!quit) {
        // Handle events to grab a quit signal so we can close cleanly
        while(SDL_PollEvent(&e) != 0) {
            onEvent(&e);
        }

        // This will pause rendering of starfield in the background
        while(inConfig) {
            // Handle events in the config window
            while(SDL_PollEvent(&e) != 0) {
                onEvent(&e);
            }

            // Clear the config window renderer
            SDL_SetRenderDrawColor(cRenderer, 0x7F, 0x7F, 0x7F, 0xFF);
            SDL_RenderClear(cRenderer);

            // Do some things

            // Present the renderer
            SDL_RenderPresent(cRenderer);

            // Still aiming at ~62 fps
            SDL_Delay(16);
        }

        // Reset renderer for the next frame
        SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(gRenderer);

        // Draw the stars
        for(int i = 0; i < starCount; i++) {
            updateStar = false;

            // Set the render color for this star (Red, blue, or white)
            stars[i].setColor(gRenderer);

            /* Draw the star to the renderer and capture the output. If     *
             * we output true, it's time to recycle this star.              */
            updateStar = stars[i].drawStar(gRenderer);

            if(updateStar) {
                stars[i].newStar(randX(generator), randY(generator), randZ(generator),
                    randR(generator), randC(generator), i);
            }
            else {
                // Update z/r
                stars[i].update();
            }
        }

        // Present the render
        SDL_RenderPresent(gRenderer);

        // 16ms delay equals ~62 fps
        SDL_Delay(16);
    }

    // Cleanup SDL systems
    close();
    return RETURN_SUCCESS;
}

int main(int argc, char** argv) {
    StarField starField;

    return starField.OnExecute();
}
