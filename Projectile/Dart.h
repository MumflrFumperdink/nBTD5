#ifndef DART_H
#define DART_H

#include <SDL/SDL.h>

#include "../utils.h"
#include "../Other/Bloon.h"
#include "../Interface/HUD.h"
#include "Projectile.h"


class Dart : public Projectile {
	private:
    short vx, vy;
		unsigned char popablity;
	public:
		Dart(short inx, short iny, double inangle, unsigned char inpopablity);
		void draw();
		bool gameLoop(std::vector<Bloon*>* bloons, HUD* h);
};

#endif
