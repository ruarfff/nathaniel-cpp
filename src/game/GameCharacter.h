#ifndef _GAMECHARACTER_H
#define _GAMECHARACTER_H

#include "SDL2/SDL.h"

class GameCharacter
{
	private:
	
	const int HEIGHT = 205;
	const int WIDTH = 64;
	
	enum class Direction {
				NORTH, NORTH_EAST, NORTH_WEST, 
				SOUTH, SOUTH_EAST, SOUTH_WEST,
				EAST, WEST 
			};

	int offSet;
	int velocity;
	int frame;
	int status;

	public:

	GameCharacter();
	void handleEvents();
	void move();
	void show();
};

#endif
