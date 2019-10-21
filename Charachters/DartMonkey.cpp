#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "../Other/Bloon.h"
#include "../Projectile/Projectile.h"
#include "../Projectile/Dart.h"
#include "../utils.h"
#include "DartMonkey.h"
#include "Character.h"

DartMonkey::DartMonkey(short inx, short iny) : Character(inx, iny) {
	range = charRange[DART_MONKEY];
	popablity = 1;
}

void DartMonkey::gameLoop(HUD* h, std::vector<Bloon*>* bloons, std::vector<Projectile*>* projectiles) {
	if (targeting == -1) {
		for (unsigned short i = 0; i < bloons->size(); i++) {
			if (!bloons->at(i)->isUnderneath() and distBetween(x, y, bloons->at(i)->getX(), bloons->at(i)->getY()) <= range) {
				angle = angleBetween(x, y, bloons->at(i)->getX(), bloons->at(i)->getY());
				targeting = i;
				startTime = oT;
				return;
			}
		}
	}
	else {
		//Firing stuff
		if ((signed)bloons->size() - 1 < targeting) {
			targeting = -1;
			stage = 1;
			return;
		}
		if (!bloons->at(targeting)->isUnderneath() and distBetween(x, y, bloons->at(targeting)->getX(), bloons->at(targeting)->getY()) <= range) angle = angleBetween(x, y, bloons->at(targeting)->getX(), bloons->at(targeting)->getY());
		unsigned long timeDifference = oT - startTime;
		if (timeDifference % (h->getInvSpeed()) == 0) {
			stage++;
		}
		if (stage > 3) {
			projectiles->push_back(new Dart(x, y, angle, popablity));
			targeting = -1;
			stage = 1;
		}
	}
}

void DartMonkey::draw(HUD* h) {
	if (selected) {
		filledCircleRGBA(screen, x, y, range, 0, 0, 0, 128);
	}

	SDL_Surface* hand;
	if (stage == 1) hand = nSDL_LoadImage(charachter_dartMonkey_arm1);
	else if (stage == 2) hand = nSDL_LoadImage(charachter_dartMonkey_arm2);
	else if (stage == 3) hand = nSDL_LoadImage(charachter_dartMonkey_arm3);

	SDL_SetColorKey(hand, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(hand->format, 0, 0, 0));
	SDL_RotateBlitAroundPoint(hand, angle, 6, x, y, -7);
	SDL_FreeSurface(hand);

	SDL_Surface* img = nSDL_LoadImage(charachter_dartMonkey);
	SDL_SetColorKey(img, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(img->format, 0, 0, 0));
	SDL_CustomBlit(img, x, y, angle);
	SDL_FreeSurface(img);

	if (upgrade[0] > 0 || upgrade[1] > 0) {
		SDL_Surface* addOn;

		if (upgrade[0] > upgrade[1]) {
			switch (upgrade[0]) {
				case 1:
					addOn = nSDL_LoadImage(charachter_dartMonkey_bandana_green);
					break;
				case 2:
					addOn = nSDL_LoadImage(charachter_dartMonkey_bandana_red);
					break;
				default:
					addOn = nSDL_LoadImage(null_Surface);
					break;
			}
		}
		else {
			switch (upgrade[1]) {
				case 1:
					addOn = nSDL_LoadImage(charachter_dartMonkey_bandana_green);
					break;
				case 2:
					addOn = nSDL_LoadImage(charachter_dartMonkey_bandana_red);
					break;
				default:
					addOn = nSDL_LoadImage(null_Surface);
					break;
			}
		}

		SDL_SetColorKey(addOn, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(addOn->format, 0, 0, 0));
		SDL_RotateBlitAroundPoint(addOn, angle, 0, x, y, -3);
		SDL_FreeSurface(addOn);
	}

	//Upgrade Menu
	if (selected) {
		SDL_Surface *upgrade1, *upgrade2;
		char title1[30], title2[30];
		unsigned char addx = 0;
		bool avail[2] = {h->getMoney() >= getUpgradeCost(0), h->getMoney() >= getUpgradeCost(1)};

		if (upgrade[0] == 0) {
			upgrade1 = nSDL_LoadImage(dartMonkey_upgrade_longerRange);
			sprintf(title1, "Longer Range\n\n\n$%d", Dart_Monkey_upgradePrices[0][0]);
		}
		else if (upgrade[0] == 1) {
			upgrade1 = nSDL_LoadImage(dartMonkey_upgrade_enhancedEyesight);
			sprintf(title1, "Enhanced Eyesight\n\n\n$%d", Dart_Monkey_upgradePrices[0][1]);
		}
		else {
			upgrade1 = nSDL_LoadImage(null_Surface);
			sprintf(title1, "Not in yet ;)");
		}

		if (upgrade[1] == 0) {
			upgrade2 = nSDL_LoadImage(dartMonkey_upgrade_sharpShots);
			sprintf(title2, "Sharp Shots\n\n\n$%d", Dart_Monkey_upgradePrices[1][0]);
		}
		else if (upgrade[1] == 1) {
			upgrade2 = nSDL_LoadImage(dartMonkey_upgrade_razerSharp);
			sprintf(title2, "Razer Sharp\nShots\n\n$%d", Dart_Monkey_upgradePrices[1][1]);
		}
		else {
			upgrade2 = nSDL_LoadImage(null_Surface);
			sprintf(title2, "Not in yet ;)");
		}

		SDL_SetColorKey(upgrade1, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade1->format, 0, 0, 0));
		SDL_SetColorKey(upgrade2, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade2->format, 0, 0, 0));

		upgradeSideBar("Dart Monkey", upgrade1, addx, title1, upgrade2, title2, avail);

		SDL_FreeSurface(upgrade1);
		SDL_FreeSurface(upgrade2);

		SDL_Surface* monkey_avatar = nSDL_LoadImage(hud_dart_monkey_small_avatar);
		SDL_CustomBlitCorner(monkey_avatar, S_WIDTH - (hud_x * 7 / 8), 35);
		SDL_FreeSurface(monkey_avatar);
	}
}

unsigned short DartMonkey::getUpgradeCost(unsigned char path) {
	return Dart_Monkey_upgradePrices[path][upgrade[path]];
}

void DartMonkey::upGrade(unsigned char path) {
	if (path == 0) {
		switch (upgrade[path]) {
			case 0:
				range = 50;
				break;
			case 1:
				range = 60;
				//can see camo bloons, add in functionality
				break;
		}
	}
	else if (path == 1) {
		switch (upgrade[path]) {
			case 0:
				popablity++;
				break;
			case 1:
				popablity += 2;
				break;
		}
	}
	upgrade[path]++;
}

void DartMonkey::select() {
	hud_x = 0;
	selected = true;
}

void DartMonkey::deselect() {
	selected = false;
}
