#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL/SDL.h>

#include "../utils.h"
#include "../Interface/HUD.h"
#include "../Other/Bloon.h"

class Bloon;
class HUD;

class Projectile {
	protected:
		float x, y;
		double angle;
		Bloon* lastPopped;
	public:
		Projectile(short inx, short iny, double inangle);
		float getX();
		float getY();
		virtual ~Projectile();
		virtual void draw() = 0;
		virtual bool gameLoop(float fElapsedTime, std::vector<Bloon*>* bloons, HUD* h) = 0;
};

#endif
