#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Texture.h"
#include "SpaceObject.h"

// Constants ==================================================================

const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;

// Functions declarations =====================================================

/**
 * @brief Initializes needed SDL features and creates the main window and
 * the renderer
 * 
 * @param window address of the pointer to the SDL_Window
 * @param renderer address of the pointer to the SDL_Renderer
 * @return true everything has been initialized
 * @return false an error occured
 */
bool init(SDL_Window** window, SDL_Renderer** renderer);

/**
 * @brief Closes SDL and frees allocated memory
 * 
 * @param window address of the pointer to the SDL_Window
 * @param renderer address of the pointer to the SDL_Renderer
 */
void close(SDL_Window** window, SDL_Renderer** renderer);

void handleCollisions(Vect* finalVect, Vect pos1, Vect pos2, float radius1, float radius2,
                      Vect vel1, Vect vel2, float mass1, float mass2);
void handlePhysics(double now, Spaceship& ship, Planet& planet);

// Functions definitions ======================================================

bool init(SDL_Window** window, SDL_Renderer** renderer) {

    // SDL Initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize. Error : %s\n", SDL_GetError());
        return false;
    }
    // Texture filtering
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        printf("Warning : linear texture filtering not enabled !\n");

    // Initlialization of the main window
    *window = SDL_CreateWindow("Spaceship", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Window could not be created. Error : %s\n", SDL_GetError());
        return false;
    }

    // Initialization of the texture renderer
    *renderer = SDL_CreateRenderer(*window, -1,
               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(!*renderer) {
        printf("Renderer could not be created. Error : %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(*renderer, 0x00, 0x00, 0x00, 0xFF);

    // Initialization of the PNG pictures loader
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_Image could not initialize. Error : %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void close(SDL_Window** window, SDL_Renderer** renderer) {
    if (*window) {
        SDL_DestroyWindow(*window);
        *window = NULL;
    }
    if (*renderer) {
        SDL_DestroyRenderer(*renderer);
        *renderer = NULL;
    }

    IMG_Quit();
    SDL_Quit();
}

float calculateNorm(Vect vector) {
    return sqrt( (pow(vector.x, 2)) + (pow(vector.y, 2)) );
}

void handleCollisions(Vect* finalVect, Vect pos1, Vect pos2, float radius1, float radius2,
                      Vect vel1, Vect vel2, float mass1, float mass2) {
    Vect d = {pos1.x-pos2.x, pos1.y-pos2.y};
    if ( !(calculateNorm(d) <= radius1 + radius2 && calculateNorm(d)) ) {
        finalVect[0] = {0,0};
        finalVect[1] = {0,0};
    }
    else {
        // (1)
        Vect u = { d.x / calculateNorm(d), d.y / calculateNorm(d) };

        float Vv_u = { vel1.x * u.x + vel1.y * u.y};
        float Vp_u = { vel2.x * u.x + vel2.y * u.y};

        // (2)
        // Ship
        Vect Vvl = { u.x * Vv_u, u.y * Vv_u};
        Vect Vvt = { vel1.x - Vvl.x, vel1.y - Vvl.y };

        // Planet
        Vect Vpl = { u.x * (vel2.x * u.x + vel2.y * u.y), u.y * (vel2.x * u.x + vel2.y * u.y) };
        Vect Vpt = { vel2.x - Vpl.x, vel2.y - Vpl.y };

        // (3)
        float Vv_prim_1 = ( (mass1 - mass2) / (mass1 + mass2) ) * Vv_u;
        float Vv_prim_2 = ( (2*mass2) / ( mass1+mass2 ) ) * Vp_u;
        float Vv_prim = Vv_prim_1 + Vv_prim_2;

        float Vp_prim_1 = ( (2*mass1) / (mass1 + mass2) ) * Vv_u;
        float Vp_prim_2 = ( (mass2 - mass1) / (mass1 + mass2) ) * Vp_u;
        float Vp_prim = Vp_prim_1 + Vp_prim_2;

        finalVect[0] = {(Vv_prim * u.x) + Vvt.x, (Vv_prim * u.y) + Vvt.y};
        finalVect[1] = {(Vp_prim * u.x) + Vpt.x, (Vp_prim * u.y) + Vpt.y};
    }
}

void handlePhysics(double now, Spaceship& ship, Planet& planet){
    static double _lastInvocationTime = 0;

    ship.managePropellant();
    
    double time = now - _lastInvocationTime;

    // Uniform motion calcul of the planet
    planet.setPosition(planet.getPosition().x + planet.getVelocity().x * time,
                       planet.getPosition().y + planet.getVelocity().y * time);
    planet.pacman(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Uniformly accelerated motion calcul of the ship
    Vect acceleration = ship.calculateAcceleration();

    ship.setVelocity(ship.getVelocity().x + (acceleration.x * time),
                     ship.getVelocity().y + (acceleration.y * time));
    ship.setPosition(ship.getPosition().x + ship.getVelocity().x * time,
                     ship.getPosition().y + ship.getVelocity().y * time);
    ship.pacman(SCREEN_WIDTH, SCREEN_HEIGHT);

    Vect colVel[2];
    handleCollisions(colVel, ship.getPosition(), planet.getPosition(),
                     ship.getRadius(), planet.getRadius(), ship.getVelocity(),
                     planet.getVelocity(), ship.getMass(), planet.getMass());
    if (colVel[0].x != 0) {
        ship.setVelocity(colVel[0].x, colVel[0].y);
        planet.setVelocity(colVel[1].x, colVel[1].y);
    }

    _lastInvocationTime = now;
}

int main(int argc, char const *argv[])  
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event e;
    bool quit = false;

    Texture background;
    Spaceship ship(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    Planet planet(SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT/2);

    if (!init(&window, &renderer))
        quit = true;

    if (!ship.loadTexture(renderer, "deathstar.png") ||
        !planet.loadTexture(renderer, "earth.png") ||
        !background.loadFromFile(renderer, "bg.png"))
        quit = true;

    const double SIMULATION_FACTOR = 10.0;
    double last_time = 0.0, t=0.0, now=0.0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                quit = true;
        }

        ship.handleEvent();

        now = SDL_GetTicks();
        t = last_time * SIMULATION_FACTOR;

        while (t < (now * SIMULATION_FACTOR) -1) {
            handlePhysics(t/SIMULATION_FACTOR, ship, planet);
            t++;
        }
        last_time = now;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render background
        background.render(renderer, 0, 0, 0);
        
        // Render ship
        ship.render(renderer);
        planet.render(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    close(&window, &renderer);
    return 0;
}
