#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

TTF_Font *load_font_from_file(std::string filename, int size);
SDL_Texture *load_image_from_file(SDL_Renderer *renderer, std::string filename);
SDL_Texture *load_text(SDL_Renderer *renderer, SDL_Color textColour,
        TTF_Font *font, std::string text);
void apply_texture(SDL_Renderer *renderer, SDL_Texture *texture,
        SDL_Rect *srcRect, SDL_Rect *destRect);

#endif
