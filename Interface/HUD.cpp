#include <SDL/SDL.h>

#include "HUD.h"
#include "Level.h"
#include "../utils.h"

HUD::HUD() {
	hud = nSDL_LoadImage(hud_back);
	plank = nSDL_LoadImage(hud_planks);

	round = charachterOffset = 0;
	speed = PAUSE;
	selecting = -1; //not selecting anything

	money = 650; //https://bloons.fandom.com/wiki/Money
	lives = 150; //https://bloons.fandom.com/wiki/Lives
}

void HUD::draw() {
	SDL_Rect hud_rect = {0, LEVEL_HEIGHT + 1};
	SDL_BlitSurface(hud, NULL, screen, &hud_rect);
	//every plank starting at x = 100
	for (unsigned char i = 0; i < 7; i++) {
		SDL_Rect plank_rect = {62 + (i * 35), 214};
		SDL_BlitSurface(plank, NULL, screen, &plank_rect);

		SDL_Rect border = {69 + (i * 35), 219, 20, 20};
		SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 72, 42, 2));

		SDL_SetAlpha(plank, SDL_SRCALPHA | SDL_RLEACCEL, 128);
		SDL_Rect plank_box = {70 + (i * 35), 220, 18, 18};
		SDL_Rect in_box = {8, 6, 18, 18};
		SDL_BlitSurface(plank, &in_box, screen, &plank_box);
		SDL_SetAlpha(plank, false, 255);

		if (selecting != i) {
			SDL_Surface* charachter = load_Charachter(static_cast<Charachter>(i + charachterOffset));
			plank_box = {70 + (i * 35), 220, 18, 18};
			SDL_BlitSurface(charachter, NULL, screen, &plank_box);
			SDL_FreeSurface(charachter);
		}
	}
	//Info
	SDL_Surface* monpic = nSDL_LoadImage(hud_money);
	SDL_SetColorKey(monpic, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(monpic->format, 0, 0, 0));
	SDL_Surface* heartpic = nSDL_LoadImage(hud_health);
	SDL_SetColorKey(heartpic, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(heartpic->format, 0, 0, 0));

	SDL_Rect money_rect = {12, LEVEL_HEIGHT + 2};
	SDL_BlitSurface(monpic, NULL, screen, &money_rect);
	SDL_Rect heart_rect = {12, LEVEL_HEIGHT + 14};
	SDL_BlitSurface(heartpic, NULL, screen, &heart_rect);

	SDL_FreeSurface(monpic);
	SDL_FreeSurface(heartpic);

	nSDL_DrawString(screen, font, 26, LEVEL_HEIGHT + 5, "%d", money);
	nSDL_DrawString(screen, font, 26, LEVEL_HEIGHT + 17, "%d", lives);

	nSDL_DrawString(screen, font, 4, 4, (round == 0 ? "Round -" : "Round %d"), round);
	//nSDL_DrawString(screen, font, 4, 20, "Selecting: %d", selecting);

	//Speed
	SDL_Surface* speedImg;
	switch (speed) {
		case PAUSE:
			speedImg = nSDL_LoadImage(hud_speed_paused);
			break;
		case SLOW:
			speedImg = nSDL_LoadImage(hud_speed_slow);
			break;
		case FAST:
			speedImg = nSDL_LoadImage(hud_speed_fast);
			break;
		case FASTER:
			speedImg = nSDL_LoadImage(hud_speed_faster);
			break;
	}
	SDL_SetColorKey(speedImg, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(speedImg->format, 0, 0, 0));
	SDL_Rect speed_rect = {5, 185};
	SDL_BlitSurface(speedImg, NULL, screen, &speed_rect);
	SDL_FreeSurface(speedImg);
}

void HUD::select(unsigned char object, Level* l) {
	if (object < interactables.size()) {
		switch (interactables.at(object).type) {
			case CHARACHTER_SELECT:
				selecting = object;
				break;
			case SPEED:
				if (oT % 4 == 0) {
					switch (speed) {
						case PAUSE:
							speed = SLOW;
							round++;
							break;
						case SLOW:
							speed = FAST;
							break;
						case FAST:
							speed = FASTER;
							break;
						case FASTER:
							speed = SLOW;
							break;
					}
				}
				deselect(l);
				break;
			case UPGRADE_MENU:
				if (selecting > 9 && oT % 2 == 0) {
					unsigned char path = object - 7;
					unsigned short cost = l->getCharachters()[selecting - 10]->getUpgradeCost(path);
					if (money >= cost) {
						l->getCharachters()[selecting - 10]->upGrade(path);
						addMoney(-(signed int)cost);
					}
				}
				break;
			case CHARACHTER:
				l->getCharachters()[object - 10]->select();
				selecting = object;
				break;
			default:
				selecting = -1;
				break;
		}
	}
}

void HUD::deselect(Level* l) {
	if (selecting > 9) { //if selecting charachter
		l->getCharachters()[selecting - 10]->deselect();
	}
	selecting = -1;
}

void HUD::addMoney(signed int add) {
	money = (unsigned int)(money + add);
}

void HUD::addMoney(unsigned int add) {
	money += add;
}

signed int HUD::getLives() {
	return lives;
}

void HUD::setLives(signed int inlives) {
	lives = inlives;
}

signed char HUD::getSelected() {
	return selecting;
}

unsigned int HUD::getMoney() {
	return money;
}

void HUD::setMoney(unsigned int mon) {
	money = mon;
}

unsigned char HUD::getSpeed() {
	return speed;
}

unsigned char HUD::getInvSpeed() {
	return 4 - speed;
}

unsigned char HUD::getRound() {
	return round - 1; //Round is offset by one for top left HUD
}

char HUD::getCharachterOffset() {
	return charachterOffset;
}

void HUD::setSpeed(unsigned char inspd) {
	speed = inspd;
}

HUD::~HUD() {
	SDL_FreeSurface(hud);
	SDL_FreeSurface(plank);
}
