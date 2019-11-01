#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "../Other/Bloon.h"
#include "../Projectile/Projectile.h"
#include "../Projectile/Boomerang.h"
#include "../utils.h"
#include "BoomerangMonkey.h"
#include "Character.h"

BoomerangMonkey::BoomerangMonkey(short inx, short iny) : Character(inx, iny) {
	range = charRange[BOOMERANG_MONKEY];
	popablity = 4;
}

void BoomerangMonkey::gameLoop(float fElapsedTime, HUD* h, std::vector<Bloon*>* bloons, std::vector<Projectile*>* projectiles) {
	if (targeting == -1) {
		for (unsigned short i = 0; i < bloons->size(); i++) {
			if (!bloons->at(i)->isUnderneath() and distBetween(x, y, bloons->at(i)->getX(), bloons->at(i)->getY()) <= range) {
				angle = angleBetween(x, y, bloons->at(i)->getX(), bloons->at(i)->getY());
				targeting = i;
				timeDifference = 0.0;
				return;
			}
		}
	}
	else {
		// Firing stuff
		if ((signed)bloons->size() - 1 < targeting) {
			targeting = -1;
			stage = 1;
			return;
		}
		if (!bloons->at(targeting)->isUnderneath() and distBetween(x, y, bloons->at(targeting)->getX(), bloons->at(targeting)->getY()) <= range) angle = angleBetween(x, y, bloons->at(targeting)->getX(), bloons->at(targeting)->getY());
		timeDifference += fElapsedTime;
		if (timeDifference >= 0.04 / h->getSpeed()) {
			timeDifference -= 0.04 / h->getSpeed();
			stage++;
		}
		if (stage > 3) {
			projectiles->push_back(new Boomerang(x, y, bloons->at(targeting)->getX(), bloons->at(targeting)->getY(), angle, popablity,  upgrade[0] >= 2, upgrade[1] >= 2));
			targeting = -1;
			stage = 1;
		}
	}
}

void BoomerangMonkey::draw() {
	if (selected) {
		filledCircleRGBA(screen, x, y, range, 0, 0, 0, 128);
	}

	SDL_Surface* hand = nullptr;
	if (stage == 1) hand = nSDL_LoadImage(charachter_boomerangMonkey_arm1);
	else if (stage == 2) hand = nSDL_LoadImage(charachter_boomerangMonkey_arm2);
	else if (stage == 3) hand = nSDL_LoadImage(charachter_boomerangMonkey_arm3);
	SDL_SetColorKey(hand, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(hand->format, 0, 0, 0));
	SDL_RotateBlitAroundPoint(hand, angle, 6, x, y, -7);
	SDL_FreeSurface(hand);

	SDL_Surface* img = nSDL_LoadImage(charachter_boomerangMonkey);
	SDL_SetColorKey(img, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(img->format, 0, 0, 0));
	SDL_CustomBlit(img, x, y, angle);
	SDL_FreeSurface(img);
}

void BoomerangMonkey::drawUp(HUD* h) {
	//Upgrade Menu
	if (selected) {
		SDL_Surface *upgrade1, *upgrade2;
		SDL_Surface* monkey_avatar = nSDL_LoadImage(hud_boomerang_monkey_small_avatar);
		char title1[30], title2[30];
		unsigned char addx = 0;
		bool avail[2] = {h->getMoney() >= getUpgradeCost(0), h->getMoney() >= getUpgradeCost(1)};

		if (upgrade[0] == 0) {
			upgrade1 = nSDL_LoadImage(boomerangMonkey_upgrade_multiTarget);
			sprintf(title1, "Multi-Target\n\n\n$%d", Boomerang_Monkey_upgradePrices[0][0]);
		}
		else if (upgrade[0] == 1) {
			addx = 15;
			upgrade1 = nSDL_LoadImage(boomerangMonkey_upgrade_glaiveThrower);
			sprintf(title1, "Glaive\nThrower\n\n$%d", Boomerang_Monkey_upgradePrices[0][1]);
		}
		else {
			upgrade1 = nSDL_LoadImage(null_Surface);
			sprintf(title1, "Not in yet ;)");
		}

		if (upgrade[1] == 0) {
			upgrade2 = nSDL_LoadImage(boomerangMonkey_upgrade_sonicBoom);
			sprintf(title2, "Sonic Boom\n\n\n$%d", Boomerang_Monkey_upgradePrices[1][0]);
		}
		else if (upgrade[1] == 1) {
			upgrade2 = nSDL_LoadImage(boomerangMonkey_upgrade_redHotRangs);
			sprintf(title2, "Red Hot\n'Rangs\n\n$%d", Boomerang_Monkey_upgradePrices[1][1]);
		}
		else {
			upgrade2 = nSDL_LoadImage(null_Surface);
			sprintf(title2, "Not in yet ;)");
		}

		SDL_SetColorKey(upgrade1, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade1->format, 0, 0, 0));
		SDL_SetColorKey(upgrade2, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade2->format, 0, 0, 0));

		upgradeSideBar(" Boomerang", upgrade1, addx, title1, upgrade2, title2, monkey_avatar, avail);

		SDL_FreeSurface(upgrade1);
		SDL_FreeSurface(upgrade2);
		SDL_FreeSurface(monkey_avatar);
	}
}

unsigned short BoomerangMonkey::getUpgradeCost(unsigned char path) {
	return Boomerang_Monkey_upgradePrices[path][upgrade[path]];;
}

void BoomerangMonkey::upGrade(unsigned char path) {
	if (path == 0) {
		switch (upgrade[path]) {
			case 0:
				//upgrade 1
				popablity = 7;
				break;
			case 1:
				//upgrade 2
				popablity = 12;
				break;
		}
	}
	else if (path == 1) {
		switch (upgrade[path]) {
			case 0:
				//upgrade 1 - frozen
				break;
			case 1:
				//upgrade 2 - lead
				break;
		}
	}
	upgrade[path]++;
}
