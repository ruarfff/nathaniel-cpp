#ifndef _BUTTON_H
#define _BUTTON_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <string>

//The button states in the sprite sheet
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

	
class Button
{
	private:


    public:
    //Initialize the variables
    Button( int x, int y, int w, int h );

    //The attributes of the button
    SDL_Rect box;
    
    //The part of the button sprite sheet that will be shown
    SDL_Rect* clip;
    
    //The clip regions of the sprite sheet
	SDL_Rect clips[ 4 ];
    
    //Handles events and set the button's sprite region
    void handle_events(SDL_Event event);

	void set_clips();
    
    //Shows the button on the screen
    void show(SDL_Texture *buttonSheet, SDL_Renderer *renderer);

};

#endif
