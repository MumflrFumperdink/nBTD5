#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <cmath>

#include "../utils.h"
#include "../Interface/HUD.h"
#include "../Other/Bloon.h"
#include "Boomerang.h"
#include "Projectile.h"

Boomerang::Boomerang(short ox, short oy, short x2, short y2, double inangle, unsigned char inpopablity, bool hot, bool glaive) : Projectile(ox, oy, 0) {
	isGlaive = glaive;
  isHot = hot;
  tin = 0;

  popablity = inpopablity;

  short x1 = ox - (short)(10 * sin(radians(inangle)));
  short y1 = oy - (short)(10 * cos(radians(inangle))); //Start in front of guy, not in him

	cx = (x1 + x2) / 2;
  cy = (y1 + y2) / 2; //Midpoint formula
  rx = sqrt(
               ((x2-x1)*(x2-x1))
             + ((y2-y1)*(y2-y1))
             ) / 2; //Dist formula, divided by 2 b/c radius
  ry = rx / 2;
  theta = atan2(y1 - y2, x1 - x2);
}

bool Boomerang::gameLoop(float fElapsedTime, std::vector<Bloon*>* bloons, HUD* h) {
	for (signed short i = bloons->size() - 1; i >= 0; i--) {
		SDL_Rect d = {x - (projectile_dart[1]/2), y - (projectile_dart[2]/2), projectile_dart[1], projectile_dart[2]};

		SDL_Surface* thisBloonImg = load_Bloon(bloons->at(i)->getType());
		SDL_Rect b = {bloons->at(i)->getX() - (thisBloonImg->w/2), bloons->at(i)->getY() - (thisBloonImg->h/2), thisBloonImg->w, thisBloonImg->h};
		SDL_FreeSurface(thisBloonImg);

		SDL_Rect inf;
		if (SDL_IntersectRect(&d, &b, &inf) && bloons->at(i) != lastPopped) {
			h->addMoney(bloonWorth[static_cast<unsigned char>(bloons->at(i)->getType())]);
      popablity--;
			lastPopped = bloons->at(i);
			if (bloons->at(i)->pop()) {
				delete bloons->at(i);
				bloons->erase(bloons->begin() + i);
			}
      return popablity == 0;
		}
	}

	// Parametric Equation of a Rotated Ellipse - https://math.stackexchange.com/a/2647450
	x = rx * cos(tin) * cos(theta) - ry * sin(tin) * sin(theta) + cx;
	y = rx * cos(tin) * sin(theta) + ry * sin(tin) * cos(theta) + cy;

	angle += 10;
	tin += 5.1 * fElapsedTime * h->getSpeed();
	return tin >= 2 * PI;
}

void Boomerang::draw() {
	SDL_Surface* img;
  if (isHot && isGlaive)
    img = nSDL_LoadImage(projectile_glaive_red);
  else if (isHot)
    img = nSDL_LoadImage(projectile_boomerang_red);
  else if (isGlaive)
    img = nSDL_LoadImage(projectile_glaive);
  else
    img = nSDL_LoadImage(projectile_boomerang);
	SDL_SetColorKey(img, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(img->format, 0, 0, 0));
	SDL_CustomBlit(img, x, y, angle);
	SDL_FreeSurface(img);
}
