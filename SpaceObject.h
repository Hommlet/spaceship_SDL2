#pragma once
#include <SDL2/SDL_image.h>
#include "Texture.h"

typedef struct _vect {
    float x;
    float y;
} Vect;

class SpaceObject {
    public:
        SpaceObject(float x, float y);

        void render(SDL_Renderer* renderer);

        bool loadTexture(SDL_Renderer* renderer, std::string path);

        void pacman(unsigned const SCREEN_WIDTH, unsigned const SCREEN_HEIGHT);

        // Accessors
        Vect getPosition() const { return _position; }
        void setPosition(float x, float y) { _position.x = x; _position.y = y; }

        Vect getVelocity() const { return _velocity; }
        void setVelocity(float x, float y) { _velocity.x = x; _velocity.y = y; }

        double getAngle() { return _angle; }
        void setRadius(double a) { _angle = a; }

        Uint8 getRadius() { return _radius; }
        void setRadius(Uint8 r) { _radius = r; }
        
        Uint8 getMass() { return _mass; }
        void setMass(Uint8 m) { _mass = m; }

    protected:
        Texture _texture;

        // Position of the object (top left corner) on the screen
        Vect _position;

        Vect _velocity;

        // Orientation of the object
        double _angle;

        Uint8 _radius;
        
        Uint8 _mass;
};

class Spaceship : public SpaceObject{
    public:
        const Uint8 PROPULSION_FACTOR = 3;
        const float PUSHING_FORCE = 0.03;

        // Constructor
        Spaceship(float x, float y);

        void render(SDL_Renderer* renderer);

        void handleEvent();

        bool loadTexture(SDL_Renderer* renderer, std::string path);

        // Accessors
        Uint8 getPropelCount() { return _propelCnt; }
        void setPropelCount(Uint8 cnt) { _propelCnt = cnt; }

        float getengineForce() { return _engineForce; }
        void setEngineForce(float e) { _engineForce = e; }

        // Special accessors
        void managePropellant();
        Vect calculateAcceleration();
        void showFire(bool show);

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