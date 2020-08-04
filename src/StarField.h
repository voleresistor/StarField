#include <random>                       // rand() and srand()
#include <SDL2/SDL.h>                   // SDL libraries
#include <iostream>                     // cout

// Size of the SDL window
#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   1024

/* STAR_FILL_RATE is multiplied by screen height to set number  *
 * of stars on screen.                                          */
#define STAR_FILL_RATE  2.5

/* CHANGE_RATE controls the rate at which z decreases.          *
 * GROWTH_RATE constrols the rate at which r increases. At 60   *
 * FPS the ideal growth rate is between 0.001 and 0.005.        */
#define CHANGE_RATE     0.005
#define GROWTH_RATE     0.001

/* MIN_Z is now deprecated in favor of direct x/y value checks  *
 * so long as z remains above 0. STAR_SIZE governs the maximum  *
 * height and width of individual stars in pixels. Larger than  *
 * 5 begins to produce some blocky stars as they approach the   *
 * screen.                                                      */
//#define MIN_Z           0.45
#define STAR_SIZE       5

// Why not set some return codes here
#define RETURN_SUCCESS      0
#define RETURN_ERROR        -1

/* The StarField struct is our basic program structure. This    *
 * makes the majority of our program variables available to the *
 * program and defines our basic methods.                       */
struct StarField {
    // SDL systems
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    SDL_Renderer* cRenderer;
    SDL_Window* cWindow;

    // We can be quitters
    bool quit;
    bool inConfig;
    bool paused;
    bool updateStar;

    // Constructor
    StarField();

    // Main program
    bool init();
    void onEvent(SDL_Event* e);
    void close();
    int OnExecute();

    // Config window
    bool onConfig();
    void closeConfig();
};

/* The Star struct describes our individual stars. This contains    *
 * the variables to describe each star's speed and trajectory as    *
 * well as several methods to manage the star's path and lifetime.  */
struct Star {
    /* Unique id. This will be recycled when the floats are         *
     * recalculated.                                                */
    int id;

    float x;    // initial X position
    float y;    // initial y position
    float z;    // acceleration value
    float r;    // size value
    float c;    // color value

    Star() {}
    Star(double x, double y, double z, double r, double c, int id) {
        this->newStar(x, y, z, r, c, id);
    }

    // Update z and r multipliers to simulate movement
    void update() {
        this->z = this->z - CHANGE_RATE;

        /* Stars won't be larger than STAR_SIZE and will grow   *
         * based on CHANGE_RATE. CHANGE_RATE needs to be        *
         * slightly modified to prevent stars from reaching     *
         * maximum size VERY early. Despite using a float to    *
         * calculate their growth, stars will still only grow   *
         * by full pixels so the growth should be gradual.      */
        if(this->r < STAR_SIZE) {
            this->r = this->r + CHANGE_RATE;
        }
    }

    /* Based on random numbers some stars may glow faintly      *
     * blue, red, or yellow. At these rates, about 3 in every   *
     * 500 stars might not be white.                            */
    void setColor(SDL_Renderer* r) {
        if(this->c < 6.1 && this->c > 5.99) {
            // color star yellow
            SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xE6, 0xFF);
        }
        else if(this->c < 1.1 && this->c > 0.99) {
            // color star blue
            SDL_SetRenderDrawColor(r, 0x99, 0xFF, 0xFF, 0xFF);
        }
        else if(this->c < 9.1 && this->c > 8.99) {
            // Color star red
            SDL_SetRenderDrawColor(r, 0xFF, 0xE6, 0xE6, 0xFF);
        }
        else {
            // color star white
            SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }

    bool drawStar(SDL_Renderer* r) {
        /* The X and Y positions are continually displaced farther and  *
         * farther outward as the Z value decreases. Adding half the    *
         * screen height/width positions the stars in an outward radius *
         * from the center of the screen rather than from the top left. */
        SDL_Rect thisStar;
        thisStar.x = (this->x / this->z) + (SCREEN_WIDTH / 2);
        thisStar.y = (this->y / this->z) + (SCREEN_HEIGHT / 2);
        thisStar.w = 1 + this->r;
        thisStar.h = 1 + this->r;

        SDL_RenderFillRect(r, &thisStar);

        /* We use this output in the main loop to determine if it's     *
         * time to recycle this particular star yet.                    */
        return resetCheck(thisStar.x, thisStar.y, this->z);
    }

    // Generate new random values for a star
    void newStar(double x, double y, double z, double r, double c, int id) {
        this->id = id;
        this->x = x;
        this->y = y;
        this->z = z;
        this->r = r;
        this->c = c;
        //std::cout << "Star " << this->id << "\n\tX: " << this->x << "\n\tY: " << this->y << "\n\tZ: " << this->z << "\n\n";
    }

    /* If the star will be drawn out of bounds on the next cycle we     *
    *  want to recycle it for a new star with new random values.        *
    * true: recycle                                                     *
    * false: OK                                                         */
    bool resetCheck(float x, float y, float z) {
        return (x >= SCREEN_WIDTH || x <= 0 || y >= SCREEN_HEIGHT || y <= 0 || z <= CHANGE_RATE);
    }
};