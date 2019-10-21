#pragma once

#include <SDL/SDL.h>

#include "../utils.h"
#include "../Other/Bloon.h"
#include "../Projectile/Projectile.h"

class Bloon;

class Character {
	protected:
		short x, y;
		unsigned char stage, range;
		double angle;
		bool selected;
		signed short targeting;
		unsigned long startTime;
		unsigned char upgrade[2] = {0, 0};
		static unsigned char hud_x;
	public:
		Character(short inx, short iny);
		void upgradeSideBar(char* title, SDL_Surface* upgrade1, unsigned char addx, char* title1, SDL_Surface* upgrade2, char* title2, bool* avail);
		virtual unsigned short getUpgradeCost(unsigned char path) = 0;
		virtual void upGrade(unsigned char path) = 0;
		virtual void draw(HUD* h) = 0;
		virtual void gameLoop(HUD* h, std::vector<Bloon*>* bloons, std::vector<Projectile*>* projectiles) = 0;
		virtual void select() = 0;
		virtual void deselect() = 0;
};