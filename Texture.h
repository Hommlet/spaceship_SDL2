#pragma once
#include <SDL2/SDL_image.h>
#include <string>

class Texture {
    public:
        // Initializes variables
        Texture();

        // Frees memory
        ~Texture();

        // Load image at specified path
        bool loadFromFile(SDL_Renderer* renderer, std::string path);

        // Deallocates texture
        void free();
        
        // Set blending
        void setAlphaMod(Uint8 alpha);

        // Renders texture
        void render(SDL_Renderer* renderer, int x, int y, double angle, SDL_Point* center = NULL);

        // Getters
        unsigned getWidth() { return _width; }
        unsigned getHeight() { return _height; }

    private:
        // Hardware texture
        SDL_Texture* _texture;

        // Image dimensions
        unsigned _width;
        unsigned _height;
};