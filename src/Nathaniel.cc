#include <iostream>
#include <string>
#include <libintl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "config.h"
#include "Button.h"
#include "GameCharacter.h"
#include "ResourceManager.h"

#define _(STRING) gettext(STRING)

#define SPRITESHEETDIR DATADIR "/spritesheets"

using namespace std;

//Screen attributes 
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Window and renderer
SDL_Window *screen = nullptr;
SDL_Renderer *renderer = nullptr;

//The textures
SDL_Texture *background = nullptr;
SDL_Texture *buttonSheet = nullptr;
SDL_Texture *upMessage = nullptr;
SDL_Texture *downMessage = nullptr;
SDL_Texture *leftMessage = nullptr;
SDL_Texture *rightMessage = nullptr;
SDL_Texture *message = nullptr;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = nullptr;

//The color of the font
SDL_Color textColour = { 255, 255, 255 };

//Game classes
Button *mainButton = nullptr;
GameCharacter *nathaniel = nullptr;

bool init() {
    //Initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return false;
    }

    //Set up the screen
    screen = SDL_CreateWindow(_("Nathaniel"),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
    renderer = SDL_CreateRenderer(screen, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //If there was an error in setting up the screen
    if (!screen || !renderer) {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 ) {
        return false;
    }

    mainButton = new Button(renderer, SPRITESHEETDIR "/buttonspritesheet.png",
            170, 120, 320, 240);
    nathaniel = new GameCharacter(renderer,
            SPRITESHEETDIR "/nathanielspritesheet.png");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //If everything initialized fine
    return true;
}

bool load_files() {
    background = load_image_from_file(renderer, DATADIR "/test.bmp");
    font = load_font_from_file(DATADIR "/fonts/lazy.ttf", 72);
    return background && font;
}

void clean_up() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(upMessage);
    SDL_DestroyTexture(downMessage);
    SDL_DestroyTexture(leftMessage);
    SDL_DestroyTexture(rightMessage);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_Quit();
}

void draw() {
    SDL_RenderClear(renderer);
    apply_texture(renderer, background, NULL, NULL);
    mainButton->show(renderer);
    nathaniel->show(renderer);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    bool quit = false;

    if (!init())
        return 1;

    if (!load_files())
        return 1;

    atexit(clean_up);

    //Setup gettext
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    //Generate the message surfaces
    upMessage = load_text(renderer, textColour, font, _("Up was pressed."));
    downMessage = load_text(renderer, textColour, font, _("Down was pressed."));
    leftMessage = load_text(renderer, textColour, font, _("Left was pressed."));
    rightMessage = load_text(renderer, textColour, font, _("Right was pressed."));

    //Apply the background
    SDL_RenderClear(renderer);
    apply_texture(renderer, background, NULL, NULL);
    SDL_RenderPresent(renderer);

    //Main loop
    while (!quit) {
        if (SDL_WaitEvent(&event)) {
            mainButton->handleEvents(event);
            nathaniel->handleEvents(event);
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        draw();
    }
    return 0;
}
