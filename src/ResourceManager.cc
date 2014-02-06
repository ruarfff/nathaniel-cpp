#include "ResourceManager.h"

using namespace std;

TTF_Font *load_font_from_file(string filename, int size) {
    return TTF_OpenFont(filename.c_str(), size);
}

SDL_Texture *load_image_from_file(SDL_Renderer *renderer, string filename) {
    SDL_Surface *loadedImage = NULL;
    loadedImage = IMG_Load(filename.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
    SDL_FreeSurface(loadedImage);
    return texture;
}

SDL_Texture *load_text(SDL_Renderer *renderer, SDL_Color textColour,
        TTF_Font *font, string text) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), textColour);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void apply_texture(SDL_Renderer *renderer, SDL_Texture *texture,
        SDL_Rect *srcRect, SDL_Rect *destRect) {
    SDL_RenderCopy(renderer, texture, srcRect, destRect);
}
