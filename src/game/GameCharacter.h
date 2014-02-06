#ifndef _GAMECHARACTER_H
#define _GAMECHARACTER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <string>

#include "ResourceManager.h"

using namespace std;

class GameCharacter {
    private:
        enum Direction { SOUTH, SOUTH_WEST, WEST, NORTH_WEST, NORTH, NORTH_EAST,
            EAST, SOUTH_EAST };

        static const int STEP_SIZE = 10;
        static const int SHEET_HEIGHT = 70;
        static const int SHEET_WIDTH = 200;
        static const int SPRITE_HEIGHT = SHEET_HEIGHT / 2;
        static const int SPRITE_WIDTH = SHEET_WIDTH / 8;

        int offset;
        int velocity;
        int frame;
        int status;
        Direction direction;

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
