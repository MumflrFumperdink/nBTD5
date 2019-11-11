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
	public:
		HUD();
		~HUD();
		void draw(float fElapsedTime) const;
		void select(long oT, unsigned char object, Level* l);
		void deselect(Level* l);
		signed char getSelected() const;
		unsigned int getMoney() const;
		void addMoney(signed int add);
		void addMoney(unsigned int add);
		float getSpeed() const;
		float getInvSpeed() const;
		void setSpeed(unsigned char inspd);
		signed int getLives() const;
		char getCharachterOffset() const;
		unsigned char getRound() const;
		void setLives(signed int inlives);
		void setMoney(unsigned int mon);
	private:
		SDL_Surface *hud, *plank;
		signed char charachterOffset/*How many right is the first*/, selecting/*Which Charachter is selected*/;
		unsigned char speed, round;
		signed int lives;
		unsigned int money;
		float sped[4] = {0.00, 1.00, 1.50, 2.00};
};
