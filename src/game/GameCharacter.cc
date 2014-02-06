#include "GameCharacter.h"

GameCharacter::GameCharacter(SDL_Renderer *renderer, string spritesheet_file) :
    direction(SOUTH), velocity(0) {
    spritesheet = load_image_from_file(renderer, spritesheet_file);
    box.x = (1280 - SPRITE_WIDTH / 2) / 2;
    box.y = (720 - SPRITE_HEIGHT / 2) / 2;
    box.w = SPRITE_WIDTH * 3;
    box.h = SPRITE_HEIGHT * 3;
    setClips();
}

GameCharacter::~GameCharacter() {
    SDL_DestroyTexture(spritesheet);
}

void GameCharacter::handleEvents(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                velocity = velocity >= 1 ? 1 : velocity + 1;
                move(direction);
                break;
            case SDLK_DOWN:
                velocity = velocity <= -1 ? -1 : velocity - 1;
                move((direction + 4) % 8);
                break;
            case SDLK_LEFT:
                direction = Direction((direction + 7) % 8);
                break;
            case SDLK_RIGHT:
                direction = Direction((direction + 1) % 8);
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
                velocity = 0;
                break;
        }
    }
}

void GameCharacter::show(SDL_Renderer *renderer) {
    int moving = velocity == 0 ? 0 : 1;
    apply_texture(renderer, spritesheet, &clips[direction][moving], &box);
}

void GameCharacter::move(int direction) {
    switch (direction) {
        case SOUTH: box.y += STEP_SIZE; break;
        case SOUTH_WEST: box.x -= STEP_SIZE; box.y += STEP_SIZE; break;
        case WEST: box.x -= STEP_SIZE; break;
        case NORTH_WEST: box.x -= STEP_SIZE; box.y -= STEP_SIZE; break;
        case NORTH: box.y -= STEP_SIZE; break;
        case NORTH_EAST: box.x += STEP_SIZE; box.y -= STEP_SIZE; break;
        case EAST: box.x += STEP_SIZE; break;
        case SOUTH_EAST: box.x += STEP_SIZE; box.y += STEP_SIZE; break;
    }
}

void GameCharacter::setClips() {
    clips[SOUTH][0].x = 0;
    clips[SOUTH][0].y = 0;
    clips[SOUTH][0].w = SPRITE_WIDTH;
    clips[SOUTH][0].h = SPRITE_HEIGHT;
    clips[SOUTH][1].x = 0;
    clips[SOUTH][1].y = SPRITE_HEIGHT;
    clips[SOUTH][1].w = SPRITE_WIDTH;
    clips[SOUTH][1].h = SPRITE_HEIGHT;

    clips[SOUTH_WEST][0].x = SPRITE_WIDTH;
    clips[SOUTH_WEST][0].y = 0;
    clips[SOUTH_WEST][0].w = SPRITE_WIDTH;
    clips[SOUTH_WEST][0].h = SPRITE_HEIGHT;
    clips[SOUTH_WEST][1].x = SPRITE_WIDTH;
    clips[SOUTH_WEST][1].y = SPRITE_HEIGHT;
    clips[SOUTH_WEST][1].w = SPRITE_WIDTH;
    clips[SOUTH_WEST][1].h = SPRITE_HEIGHT;

    clips[NORTH][0].x = SPRITE_WIDTH * 2;
    clips[NORTH][0].y = 0;
    clips[NORTH][0].w = SPRITE_WIDTH;
    clips[NORTH][0].h = SPRITE_HEIGHT;
    clips[NORTH][1].x = SPRITE_WIDTH * 2;
    clips[NORTH][1].y = SPRITE_HEIGHT;
    clips[NORTH][1].w = SPRITE_WIDTH;
    clips[NORTH][1].h = SPRITE_HEIGHT;

    clips[WEST][0].x = SPRITE_WIDTH * 3;
    clips[WEST][0].y = 0;
    clips[WEST][0].w = SPRITE_WIDTH;
    clips[WEST][0].h = SPRITE_HEIGHT;
    clips[WEST][1].x = SPRITE_WIDTH * 3;
    clips[WEST][1].y = SPRITE_HEIGHT;
    clips[WEST][1].w = SPRITE_WIDTH;
    clips[WEST][1].h = SPRITE_HEIGHT;

    clips[SOUTH_EAST][0].x = SPRITE_WIDTH * 4;
    clips[SOUTH_EAST][0].y = 0;
    clips[SOUTH_EAST][0].w = SPRITE_WIDTH;
    clips[SOUTH_EAST][0].h = SPRITE_HEIGHT;
    clips[SOUTH_EAST][1].x = SPRITE_WIDTH * 4;
    clips[SOUTH_EAST][1].y = SPRITE_HEIGHT;
    clips[SOUTH_EAST][1].w = SPRITE_WIDTH;
    clips[SOUTH_EAST][1].h = SPRITE_HEIGHT;

    clips[NORTH_EAST][0].x = SPRITE_WIDTH * 5;
    clips[NORTH_EAST][0].y = 0;
    clips[NORTH_EAST][0].w = SPRITE_WIDTH;
    clips[NORTH_EAST][0].h = SPRITE_HEIGHT;
    clips[NORTH_EAST][1].x = SPRITE_WIDTH * 5;
    clips[NORTH_EAST][1].y = SPRITE_HEIGHT;
    clips[NORTH_EAST][1].w = SPRITE_WIDTH;
    clips[NORTH_EAST][1].h = SPRITE_HEIGHT;

    clips[EAST][0].x = SPRITE_WIDTH * 6;
    clips[EAST][0].y = 0;
    clips[EAST][0].w = SPRITE_WIDTH;
    clips[EAST][0].h = SPRITE_HEIGHT;
    clips[EAST][1].x = SPRITE_WIDTH * 6;
    clips[EAST][1].y = SPRITE_HEIGHT;
    clips[EAST][1].w = SPRITE_WIDTH;
    clips[EAST][1].h = SPRITE_HEIGHT;

    clips[NORTH_WEST][0].x = SPRITE_WIDTH * 7;
    clips[NORTH_WEST][0].y = 0;
    clips[NORTH_WEST][0].w = SPRITE_WIDTH;
    clips[NORTH_WEST][0].h = SPRITE_HEIGHT;
    clips[NORTH_WEST][1].x = SPRITE_WIDTH * 7;
    clips[NORTH_WEST][1].y = SPRITE_HEIGHT;
    clips[NORTH_WEST][1].w = SPRITE_WIDTH;
    clips[NORTH_WEST][1].h = SPRITE_HEIGHT;
}
