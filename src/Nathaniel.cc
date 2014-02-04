#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Button.h"
#include "GameCharacter.h"

using namespace std;

//Screen attributes 
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Window and renderer
SDL_Window *screen = NULL;
SDL_Renderer *renderer = NULL;

//The textures
SDL_Texture *background = NULL;
SDL_Texture *upMessage = NULL;
SDL_Texture *downMessage = NULL;
SDL_Texture *leftMessage = NULL;
SDL_Texture *rightMessage = NULL;
SDL_Texture *message = NULL;

SDL_Texture *buttonSheet = NULL;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 255, 255, 255 };

//Make the button
Button mainButton( 170, 120, 320, 240 );

SDL_Texture *load_image(string filename)
{
    //The image that's loaded
    SDL_Surface *loadedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
    SDL_FreeSurface(loadedImage);
    return texture;
}

SDL_Texture *textToTexture(string text) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void apply_texture(SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *destRect) {
    SDL_RenderCopy(renderer, texture, srcRect, destRect);
}

void apply_texture(SDL_Texture *texture) {
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return false;
    }

    //Set up the screen
    screen = SDL_CreateWindow("Nathaniel",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
    renderer = SDL_CreateRenderer(screen, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //If there was an error in setting up the screen
    if( screen == NULL || renderer == NULL ) {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 ) {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( DATADIR "/test.bmp" );
    buttonSheet = load_image( DATADIR "/button.png" );

    //Open the font
    font = TTF_OpenFont( DATADIR "/fonts/lazy.ttf", 72 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If there was a problem in loading the button sprite sheet
    if( buttonSheet == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(upMessage);
    SDL_DestroyTexture(downMessage);
    SDL_DestroyTexture(leftMessage);
    SDL_DestroyTexture(rightMessage);
    SDL_DestroyTexture(buttonSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
}

int main(int argc, char* argv[]) {

    bool quit = false;
    SDL_Rect srcRect, destRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = SCREEN_WIDTH;
    srcRect.h = SCREEN_HEIGHT;
    destRect.x = 20;
    destRect.y = 20;
    destRect.w = SCREEN_WIDTH;
    destRect.h = SCREEN_HEIGHT;

    if(!init())
        return 1;

    if(!load_files())
        return 1;

    atexit(clean_up);

    //Generate the message surfaces
    upMessage = textToTexture("Up was pressed.");
    downMessage = textToTexture("Down was pressed.");
    leftMessage = textToTexture("Left was pressed.");
    rightMessage = textToTexture("Right was pressed.");

    //Apply the background
    SDL_RenderClear(renderer);
    apply_texture(background);
    SDL_RenderPresent(renderer);

    //Clip the sprite sheet
    mainButton.set_clips();    

    while (!quit) {
        if (SDL_WaitEvent(&event)) {
            mainButton.handle_events(event);
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: message = upMessage; break;
                    case SDLK_DOWN: message = downMessage; break;
                    case SDLK_LEFT: message = leftMessage; break;
                    case SDLK_RIGHT: message = rightMessage; break;
                }
            }
            else if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_QueryTexture(message, 0, 0, &srcRect.w, &srcRect.h);
        SDL_RenderClear(renderer);
        apply_texture(background);
        apply_texture(message, &srcRect, &srcRect);
        apply_texture(buttonSheet, mainButton.clip, &mainButton.box);
        SDL_RenderPresent(renderer);
    }
    return 0;
}
