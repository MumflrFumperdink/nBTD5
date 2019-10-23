#ifndef BOOMERANG_H
#define BOOMERANG_H

#include <SDL/SDL.h>

#include "../utils.h"
#include "../Other/Bloon.h"
#include "../Interface/HUD.h"
#include "Projectile.h"


class Boomerang : public Projectile {
        private:
                bool isGlaive, isHot;
                unsigned char popablity;
                //Using rotated ellipse for path
                unsigned short cx;
                unsigned short cy;
                float rx;
                float ry;
                float theta;
                //Time input
                float tin;
        public:
        	Boomerang(short ox1, short oy1, short x2, short y2, double inangle, unsigned char inpopablity, bool hot, bool glaive);
        	void draw();
        	bool gameLoop(float fElapsedTime, std::vector<Bloon*>* bloons, HUD* h);
};

#endif
