#pragma once

#include <vector>
#include <SDL/SDL.h>

#include "../utils.h"
#include "../Interface/HUD.h"

/*
 * A lot of the information comes from the
 * Bloons wiki: https://bloons.fandom.com/wiki/Bloons_Wiki
 *
 * Info on lives:
 * https://forums.ninjakiwi.com/forum/ninja-kiwi-games/bloons-td-5/111778-how-much-lifes-does-each-bloon-take-away
 *
 * Also bonus info:
 * http://bloons-tower-defense-5.blogspot.com/
 *
 */

class HUD;

class Bloon {
	private:
		float x, y; //position
		unsigned char nextCheckpoint; //position heading to
		bool goingVer, goingPos; //going vertically or horizontally, then positive or negative
		bool isUnder; //going under tunnel
		BloonType type; //Type of bloon
	public:
		Bloon(BloonType intype);
		bool gameLoop(HUD* h); //returns true if poped
		void draw();
		float getX();
		float getY();
		unsigned char getNextCheckpoint();
		BloonType getType();
		bool getGoingPos();
		bool getGoingVer();
		bool switchToNextCheckpoint();
		bool isUnderneath();
		void goUnder();
		void goOver();
		bool pop();
};
