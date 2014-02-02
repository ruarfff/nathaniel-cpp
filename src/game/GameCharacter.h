#ifndef _GAMECHARACTER_H
#define _GAMECHARACTER_H

#include "SDL/SDL.h"

class GameCharacter
{
	private:
	
	int offSet;
	int velocity;
	int frame;
	int status;

	const int WIDTH = 64;
	const int HEIGHT = 205;

	public:

	GameCharacter();
	void handleEvents();
	void move();
	void show();
};

#endif
