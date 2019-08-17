#include "Texture.h"

Texture::Texture()
{
	//Initialize
	_texture = NULL;
	_width = 0;
	_height = 0;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(!loadedSurface)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n",
               path.c_str(), IMG_GetError());
        return false;
	}

    //Color key image
    /*SDL_SetColorKey(loadedSurface, SDL_TRUE,
                    SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));*/

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(!newTexture)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n",
                path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    //Get image dimensions
    _width = loadedSurface->w;
    _height = loadedSurface->h;

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

	//Return success
	_texture = newTexture;
	
    return true;
}

void Texture::free()
{
	//Free texture if it exists
	if(_texture)
	{
		SDL_DestroyTexture(_texture);
		_texture = NULL;
		_width = 0;
		_height = 0;
	}
}

void Texture::setAlphaMod(Uint8 alpha) {
	SDL_SetTextureAlphaMod(_texture, alpha);
}

void Texture::render(SDL_Renderer* renderer, int x, int y, double angle,
					 SDL_Point* center)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, (int)_width, (int)_height};

	//Render to screen
	SDL_RenderCopyEx(renderer, _texture, NULL, &renderQuad, angle, center,
                     SDL_FLIP_NONE);
}