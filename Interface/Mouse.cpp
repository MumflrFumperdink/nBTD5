#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "Mouse.h"
#include "HUD.h"
#include "../utils.h"

Mouse::Mouse() {
	x = y = 0;
	cursor = nSDL_LoadImage(mouse_cursor);
	SDL_SetColorKey(cursor, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(cursor->format, 0, 0, 0));
}

bool Mouse::intersects(std::vector<SDL_Rect>* rects) {
	SDL_Rect mRect = {x, y, 1, 1};
	SDL_Rect intersection;
	for (unsigned char i = 0; i < rects->size(); i++) {
		auto a = rects->at(i);
		if (SDL_IntersectRect(&a, &mRect, &intersection)) return true;
	}
	return false;
}

void Mouse::draw(HUD* h) {
	signed char touching = whichInteractable(h);
	signed char touching_off = touching + h->getCharachterOffset();
	signed char selecting = h->getSelected() + h->getCharachterOffset();

	if (h->getSelected() >= 0 && h->getSelected() < 7) {
		SDL_Surface* charachter = load_Charachter(static_cast<Charachter>(selecting));
		filledCircleRGBA(screen, x, y, charRange[selecting], h->getMoney() >= (unsigned int)charCost[selecting] && !intersects(&trackCheckpoints) ? 0 : 255, 0, 0, 128);
		SDL_CustomBlit(charachter, x, y, 0);
		SDL_FreeSurface(charachter);
	}

	if ((touching >= 0 && (touching < 7 || touching > 8)) || ((touching == 7 || touching == 8) && h->getSelected() > 9)) {
		SDL_Surface* touching_cursor = nSDL_LoadImage(mouse_cursor_touch);
		SDL_SetColorKey(touching_cursor, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(touching_cursor->format, 0, 0, 0));
		SDL_Rect pos_rect = {x - 6, y - 1};
		SDL_BlitSurface(touching_cursor, NULL, screen, &pos_rect);
		SDL_FreeSurface(touching_cursor);

		if (touching >= 0 && touching < 7) {
			nSDL_DrawString(screen, font, x + 5, y - 5, "$%d", charCost[touching_off]);
		}
	}
	else {
		SDL_Rect pos_rect = {x, y};
		SDL_BlitSurface(cursor, NULL, screen, &pos_rect);
	}
}

void Mouse::update() {
	touchpad_scan(&tpad_report);
}

bool Mouse::hasMoved() {
	if (tpad_report.x != 0 and tpad_report.y != 239) { //for some reason defaults to these coordinates
		if (tpad_report.x != x or tpad_report.y != y) {
			x = tpad_report.x * (S_WIDTH - 1) / tpad_info->width;
			y = (S_HEIGHT - 1) - (tpad_report.y * (S_HEIGHT - 1) / tpad_info->height);
			return true;
		}
	}
	return false;
}

bool Mouse::isTouchingInteractable() {
	SDL_Rect mouse = {x, y, 1, 1};
	SDL_Rect intersection; //used to find intersection area of rects
	for (unsigned char i = 0; i < interactables.size(); i++) {
		if (SDL_IntersectRect(&interactables.at(i).pos, &mouse, &intersection)) {
			return true;
		}
	}
	return false;
}

signed char Mouse::whichInteractable(HUD* h) {
	SDL_Rect mouse = {x, y, 1, 1};
	SDL_Rect intersection; //used to find intersection area of rects
	for (unsigned char i = 0; i < interactables.size(); i++) {
		if (SDL_IntersectRect(&interactables.at(i).pos, &mouse, &intersection)) {
			if (!(h->getSelected() < 0 && (i == 7 || i == 8))) return i;
		}
	}
	return -1;
}

bool Mouse::hasClicked() {
	return tpad_report.pressed == true;
}

short Mouse::getX() {
	return x;
}

short Mouse::getY() {
	return y;
}

void Mouse::quit() {
	SDL_FreeSurface(cursor);
}
