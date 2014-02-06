#ifndef _BUTTON_H
#define _BUTTON_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <string>

#include "ResourceManager.h"

//The button states in the sprite sheet
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

	
class Button {
    private:
        SDL_Texture *spritesheet;
        SDL_Rect box;
        SDL_Rect clips[4];
        SDL_Rect* currentClip;

        void setClips();

    public:
        Button(SDL_Renderer *renderer, std::string spritesheet_file,
                int x, int y, int w, int h);
        ~Button();
        void handleEvents(SDL_Event event);
        void show(SDL_Renderer *renderer);

};

#endif
