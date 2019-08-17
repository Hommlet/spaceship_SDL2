#include <cmath>

#include "Spaceship.h"

SpaceObject::SpaceObject(float x, float y):
           _position({x, y}), _velocity({0, 0}), _angle(0){}

Spaceship::Spaceship(float x, float y) : SpaceObject(x, y) {
    _radius = 20;
}
Planet::Planet(float x, float y) : SpaceObject(x, y) {
    _radius = 40;
}

bool SpaceObject::loadTexture(SDL_Renderer* renderer, std::string path) {
    if (!_texture.loadFromFile(renderer, path))
        return false;

    return true;
}

bool Spaceship::loadTexture(SDL_Renderer* renderer, std::string path) {
    SpaceObject::loadTexture(renderer, path);
    if (!_flamesTexture.loadFromFile(renderer, "flames.png"))
        return false;
    
    return true;
}

void Spaceship::handleEvent(){
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    if (keyStates[SDL_SCANCODE_LEFT]) {
        _angle -= M_PI/20;
    }
    else if (keyStates[SDL_SCANCODE_RIGHT]) {
        _angle += M_PI/20;
    }
    else if (keyStates[SDL_SCANCODE_UP]) {
        _propelCnt = PROPULSION_FACTOR;
    }
}

void SpaceObject::render(SDL_Renderer* renderer){
    _texture.render(renderer, round(_position.x), round(_position.y),
                    (_angle * 180) / M_PI, NULL);
}

void Spaceship::render(SDL_Renderer* renderer){
    if (_engineForce) {
        SDL_Point p = {(int)_flamesTexture.getWidth() + (int)_texture.getWidth()/2,
                       (int)_flamesTexture.getHeight()/2};
        _flamesTexture.render(renderer,
                              round(_position.x - _flamesTexture.getWidth()),
                              round(_position.y), (_angle * 180) / M_PI, &p);
    }
    SpaceObject::render(renderer);
}

void Spaceship::handlePhysics(double now){
    static double _lastInvocationTime = 0;
    
    if (_propelCnt > 0) {
        _engineForce = PUSHING_FORCE;
        _propelCnt --;
    }
    else {
        _engineForce = 0;
    }
    
    double time = now - _lastInvocationTime;

    // Uniformly accelerated motion calcul of the ship
    Coord acceleration;
    acceleration.x = (_engineForce / MASS) * cos(_angle);
    acceleration.y = (_engineForce / MASS) * sin(_angle);
    _velocity.x = _velocity.x + (acceleration.x * time);
    _velocity.y = _velocity.y + (acceleration.y * time);

    _position.x = _position.x + _velocity.x * time;
    _position.y = _position.y + _velocity.y * time;

    _lastInvocationTime = now;

    // Pacman effect
    if (_position.x >= 800+100)
        _position.x = 0;
    else if (_position.x <= -100)
        _position.x = 800;

    if (_position.y >= 600+100)
        _position.y = 0;
    else if (_position.y <= -100)
        _position.y = 600;

}