#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "../utils.h"
#include "../Interface/HUD.h"
#include "../Other/Bloon.h"
#include "Dart.h"
#include "Projectile.h"

Dart::Dart(short inx, short iny, double inangle, unsigned char inpopablity) : Projectile(inx, iny, inangle) {
	constexpr const unsigned char speed = 240;
	vx = -(short)(speed * sin(radians(angle)));
	vy = -(short)(speed * cos(radians(angle)));
	popablity = inpopablity;
}

bool Dart::gameLoop(float fElapsedTime, std::vector<Bloon*>* bloons, HUD* h) {
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
	x += vx * fElapsedTime * h->getSpeed();
	y += vy * fElapsedTime * h->getSpeed();
	return false;
}

void Dart::draw() {
	SDL_Surface* img = nSDL_LoadImage(projectile_dart);
	SDL_SetColorKey(img, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(img->format, 0, 0, 0));
	SDL_CustomBlit(img, x, y, angle);
	SDL_FreeSurface(img);
}
