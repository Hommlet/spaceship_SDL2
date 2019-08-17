#pragma once
#include <SDL2/SDL_image.h>
#include "Texture.h"

typedef struct _coord {
    float x;
    float y;
} Coord;

class SpaceObject {
    public:
        const Uint8 MASS = 1;

        SpaceObject(float x, float y);

        void render(SDL_Renderer* renderer);

        //void handlePhysics(double timeNow);

        bool loadTexture(SDL_Renderer* renderer, std::string path);

    protected:
        const Uint8 RADIUS = 20;

        Texture _texture;

        // Position of the object (top left corner) on the screen
        Coord _position;

        Coord _velocity;

        Uint8 _radius;

        // Orientation of the object
        double _angle;
};

class Spaceship : public SpaceObject{
    public:
        const Uint8 PROPULSION_FACTOR = 3;
        const float PUSHING_FORCE = 0.0003;

        // Constructor
        Spaceship(float x, float y);

        void render(SDL_Renderer* renderer);

        void handlePhysics(double timeNow);

        void handleEvent();

        bool loadTexture(SDL_Renderer* renderer, std::string path);

        // Accessors
        Uint8 getPropelCount() { return _propelCnt; }
        void setPropelCount(Uint8 cnt) { _propelCnt = cnt; }

    private:
        Texture _flamesTexture;

        // Propel counter
        Uint8 _propelCnt;

        // Engine force (managed by the propelCnt)
        float _engineForce;
};

class Planet : public SpaceObject {
    public:
        Planet(float x, float y);
};