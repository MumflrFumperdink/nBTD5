#pragma once

#include <SDL/SDL.h>

#include "Level.h"
#include "../utils.h"

#define PAUSE 0
#define SLOW 1
#define FAST 2
#define FASTER 3

class Level;

class HUD {
	private:
		SDL_Surface *hud, *plank;
		signed char charachterOffset/*How many right is the first*/, selecting/*Which Charachter is selected*/;
		unsigned char speed, round;
		signed int lives;
		unsigned int money;

	public:
		HUD();
		~HUD();
		void draw(float fElapsedTime);
		void select(unsigned char object, Level* l);
		void deselect(Level* l);
		signed char getSelected();
		unsigned int getMoney();
		void addMoney(signed int add);
		void addMoney(unsigned int add);
		unsigned char getSpeed();
		unsigned char getInvSpeed();
		void setSpeed(unsigned char inspd);
		signed int getLives();
		char getCharachterOffset();
		unsigned char getRound();
		void setLives(signed int inlives);
		void setMoney(unsigned int mon);
};
