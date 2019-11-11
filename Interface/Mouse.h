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
		void draw(HUD* h) const;
		void update();
		signed char whichInteractable(HUD* h) const;
		bool isTouchingInteractable() const;
		bool intersects(std::vector<SDL_Rect>* rects) const;
		bool hasClicked() const;
		bool hasMoved();
		short getX() const;
		short getY() const;
		void quit();
};
