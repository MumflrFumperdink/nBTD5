#pragma once

#include <SDL/SDL.h>

#include "Character.h"
#include "../utils.h"
#include "../Projectile/Projectile.h"
#include "../Other/Bloon.h"

class DartMonkey : public Character {
	private:
		unsigned char popablity;
	public:
		DartMonkey(short inx, short iny);
		unsigned short getUpgradeCost(unsigned char path);
		void upGrade(unsigned char path);
		void draw();
    void drawUp(HUD* h);
		void gameLoop(float fElapsedTime, HUD* h, std::vector<Bloon*>* bloons, std::vector<Projectile*>* projectiles);
};
