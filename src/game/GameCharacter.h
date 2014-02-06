#ifndef _GAMECHARACTER_H
#define _GAMECHARACTER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <string>

#include "ResourceManager.h"

using namespace std;

class GameCharacter {
    private:
        static const int SOUTH = 0;
        static const int SOUTH_WEST = 1;
        static const int WEST = 2;
        static const int NORTH_WEST = 3;
        static const int NORTH = 4;
        static const int NORTH_EAST = 5;
        static const int EAST = 6;
        static const int SOUTH_EAST = 7;

        static const int STEP_SIZE = 10;
        static const int SHEET_HEIGHT = 70;
        static const int SHEET_WIDTH = 200;
        static const int SPRITE_HEIGHT = SHEET_HEIGHT / 2;
        static const int SPRITE_WIDTH = SHEET_WIDTH / 8;

        int offset;
        int velocity;
        int frame;
        int status;
        int direction;

        SDL_Texture *spritesheet;
        SDL_Rect box;
        SDL_Rect clips[8][2];

        void setClips();
    public:
        GameCharacter(SDL_Renderer *renderer, string spritesheet_file);
        ~GameCharacter();
        void handleEvents(SDL_Event event);
        void show(SDL_Renderer *renderer);
        void move(int direction);
};

#endif
