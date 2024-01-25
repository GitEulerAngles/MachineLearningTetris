#include "basic.h"

void basic::texture::loadMedia(SDL_Renderer* r, std::string path) {
    //Load PNG texture
    gTexture = loadTexture(path, r);
    if (gTexture == NULL)
    {
        printf("Failed to load texture image!\n");
    }
}
void basic::texture::drawTexture(SDL_Renderer* r) {
    try {
        SDL_RenderCopy(r, gTexture, &imageCoords, &screenCoords);
    }
    catch (const std::exception& e) {
        std::cout << "Texture error" << std::endl;
    }
}
SDL_Texture* basic::texture::loadTexture(std::string path, SDL_Renderer* r) {
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(r, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
basic::Vector2i addVector2i(basic::Vector2i v1, basic::Vector2i v2) {
    return basic::Vector2i(v1.x + v2.x, v1.y + v2.y);
}
basic::Vector2i subVector2i(basic::Vector2i v1, basic::Vector2i v2) {
    return basic::Vector2i(v1.x - v2.x, v1.y - v2.y);
}
