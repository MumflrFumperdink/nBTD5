#include <SDL/SDL.h>

#include "Bloon.h"
#include "../utils.h"
#include "../Interface/HUD.h"

Bloon::Bloon(BloonType intype) {
	x = checkpoints.at(0)[0];
	y = checkpoints.at(0)[1];
	type = intype;
	nextCheckpoint = 1;
	goingVer = (x == checkpoints.at(nextCheckpoint)[0]);
	goingPos = goingVer ? y < checkpoints.at(nextCheckpoint)[1] : x < checkpoints.at(nextCheckpoint)[0];
	isUnder = false;
}

void Bloon::draw() {
	if (!isUnder) {
		SDL_Surface* img = load_Bloon(type);
		SDL_CustomBlit(img, x, y, 0);
		SDL_FreeSurface(img);
	}
}

bool Bloon::gameLoop(HUD* h) {
	if (goingVer) {
		if (goingPos) { //Going Down
			if (y >= checkpoints.at(nextCheckpoint)[1]) {
				y = checkpoints.at(nextCheckpoint)[1];
				return switchToNextCheckpoint();
			}
			else {
				y += bloonSpeed[static_cast<unsigned char>(type)] * h->getSpeed();
			}
		}
		else { //Going Up
			if (y <= checkpoints.at(nextCheckpoint)[1]) {
				y = checkpoints.at(nextCheckpoint)[1];
				return switchToNextCheckpoint();
			}
			else {
				y -= bloonSpeed[static_cast<unsigned char>(type)] * h->getSpeed();
			}
		}
	}
	else {
		if (goingPos) { //Going Right
			if (x >= checkpoints.at(nextCheckpoint)[0]) {
				x = checkpoints.at(nextCheckpoint)[0];
				return switchToNextCheckpoint();
			}
			else {
				x += bloonSpeed[static_cast<unsigned char>(type)] * h->getSpeed();
			}
		}
		else { //Going Left
			if (x <= checkpoints.at(nextCheckpoint)[0]) {
				x = checkpoints.at(nextCheckpoint)[0];
				return switchToNextCheckpoint();
			}
			else {
				x -= bloonSpeed[static_cast<unsigned char>(type)] * h->getSpeed();
			}
		}
	}

	return false;
}

bool Bloon::switchToNextCheckpoint() {
	if (nextCheckpoint + 1 < checkpoints.size()) {
		nextCheckpoint++;
		goingVer = (x == checkpoints.at(nextCheckpoint)[0]);
		goingPos = goingVer ? y < checkpoints.at(nextCheckpoint)[1] : x < checkpoints.at(nextCheckpoint)[0];
		return false;
	}
	return true;
}

bool Bloon::pop() {
	SDL_Surface* pop = nSDL_LoadImage(popEffect);
	SDL_SetColorKey(pop, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(pop->format, 0, 0, 0));
	SDL_CustomBlit(pop, x, y, 0);
	SDL_FreeSurface(pop);

	if (type == RED) return true;
	type = static_cast<BloonType>(static_cast<unsigned char>(type) - 1);
	return false;
}

void Bloon::goUnder() {
	isUnder = true;
}

void Bloon::goOver() {
	isUnder = false;
}

unsigned char Bloon::getNextCheckpoint() {
	return nextCheckpoint;
}

BloonType Bloon::getType() {
	return type;
}

bool Bloon::isUnderneath() {
	return isUnder;
}

bool Bloon::getGoingPos() {
	return goingPos;
}

bool Bloon::getGoingVer() {
	return goingVer;
}

float Bloon::getX() {
	return x;
}

float Bloon::getY() {
	return y;
}
