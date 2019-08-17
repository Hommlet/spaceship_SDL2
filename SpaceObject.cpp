#include <cmath>

#include "SpaceObject.h"

SpaceObject::SpaceObject(float x, float y):
           _position({x, y}), _velocity({0, 0}), _angle(0){}

Spaceship::Spaceship(float x, float y) : SpaceObject(x, y) {
    _radius = 20;
    _mass = 3;
}
Planet::Planet(float x, float y) : SpaceObject(x, y) {
    _radius = 40;
    _mass = 5;
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
    if (keyStates[SDL_SCANCODE_UP]) {
        _propelCnt = PROPULSION_FACTOR;
        showFire(true);
    } else
        showFire(false);
}

void SpaceObject::render(SDL_Renderer* renderer){
    _texture.render(renderer, round(_position.x - _texture.getWidth()/2 ),
                    round(_position.y - _texture.getHeight()/2), (_angle * 180) / M_PI, NULL);
}

void Spaceship::render(SDL_Renderer* renderer){
    SDL_Point p = {(int)_flamesTexture.getWidth() + (int)_texture.getWidth()/2,
                    (int)_flamesTexture.getHeight()/2};
    _flamesTexture.render(renderer,
                            round(_position.x - _flamesTexture.getWidth() - _texture.getWidth()/2),
                            round(_position.y- _texture.getHeight()/2), (_angle * 180) / M_PI, &p);
    SpaceObject::render(renderer);
}

void Spaceship::managePropellant() {
    if (_propelCnt > 0) {
        _engineForce = PUSHING_FORCE;
        _propelCnt--;
    }
    else {
        _engineForce = 0;
    }
}

Vect Spaceship::calculateAcceleration() {
    Vect acc;
    acc.x = (_engineForce / _mass) * cos(_angle);
    acc.y = (_engineForce / _mass) * sin(_angle);
    
    return acc;
}

void SpaceObject::pacman(unsigned const SCREEN_WIDTH,
                         unsigned const SCREEN_HEIGHT) {
    if (_position.x >= SCREEN_WIDTH+100)
        _position.x = 0;
    else if (_position.x <= -100)
        _position.x = SCREEN_WIDTH;

    if (_position.y >= SCREEN_HEIGHT+100)
        _position.y = 0;
    else if (_position.y <= -100)
        _position.y = SCREEN_HEIGHT;
}

void Spaceship::showFire(bool show) {
    _flamesTexture.setAlphaMod((show)?0xFF:0x00);
}