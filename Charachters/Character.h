#pragma once

#include <SDL/SDL.h>

#include "../utils.h"
#include "../Other/Bloon.h"
#include "../Projectile/Projectile.h"

class Bloon;
class Projectile;

class Character {
	protected:
		short x, y;
		unsigned char stage, range;
		double angle;
		bool selected;
		bool lefty;
		signed short targeting;
		float timeDifference;
		unsigned char upgrade[2] = {0, 0};
		static unsigned char hud_x;
	public:
		Character(short inx, short iny);
		void upgradeSideBar(const char* title, SDL_Surface* upgrade1, unsigned char addx, char* title1, SDL_Surface* upgrade2, char* title2, SDL_Surface* avatar, bool* avail);
		virtual ~Character();
		virtual unsigned short getUpgradeCost(unsigned char path) = 0;
		virtual void upGrade(unsigned char path) = 0;
		virtual void draw() = 0;
		virtual void drawUp(HUD* h) = 0;
		virtual void gameLoop(float fElapsedTime, HUD* h, std::vector<Bloon*>* bloons, std::vector<Projectile*>* projectiles) = 0;
		virtual void select();
		virtual void deselect();
};
