#pragma once

#include <vector>
#include <SDL/SDL.h>

#include "HUD.h"
#include "../utils.h"

class HUD;

class Mouse {
	private:
		short x, y;
		touchpad_report_t tpad_report;
		SDL_Surface* cursor;
	public:
		Mouse();
		void draw(HUD* h);
		void update();
		signed char whichInteractable(HUD* h);
		bool isTouchingInteractable();
		bool intersects(std::vector<SDL_Rect>* rects);
		bool hasClicked();
		bool hasMoved();
		short getX();
		short getY();
		void quit();
};
