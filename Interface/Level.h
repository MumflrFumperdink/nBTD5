#pragma once

#include <SDL/SDL.h>
#include <map>
#include <vector>

#include "../Charachters/Character.h"
#include "../Projectile/Projectile.h"
#include "../Other/Bloon.h"
#include "Mouse.h"
#include "HUD.h"
#include "../utils.h"

class Mouse;
class Character;

class Level {
	public:
		Level(LevelName l);
		~Level();
		bool hasRoundCleared(unsigned char currentRound);
		void draw(HUD* h);
		void loop(HUD* h);
		void addCharachter(Mouse* m, HUD* h);
		std::vector<Character*>& getCharachters();
		std::vector<SDL_Rect>* getTrackCheckpoints();
	private:
		SDL_Surface* level;

		LevelName name;
		std::vector<Character*> charchters;
		std::vector<Projectile*> projectiles;
		std::vector<Bloon*> bloons;
		std::vector<unsigned char> underneathCheckpoints;

		//The bloons in each round (info from https://btd-5.fandom.com/wiki/Round)
		//  RED        BLUE      GREEN     YELLOW    PINK
		unsigned short rounds[16][numBloonTypes][2] = { //round, bloonType(as index)/amount of bloonType that round and timeInterval between
			{{20, 5}},
			{{30, 5}},
			{{20, 5},  {20, 12}},
			{{30, 5},  {15, 12}},
			{{5,  2},  {25, 6}},
			{{15, 5},  {15, 8},   {4, 9}},
			{{20, 5},  {25, 8},   {5, 10}},
			{{10, 8},  {20, 10},  {14, 12}},
			{{},       {},        {30, 8}},
			{{},       {102, 10}},
			{{10, 11}, {10, 13},  {12, 8},  {2, 30}},
			{{},       {15, 8},   {10, 11}, {5, 15}},
			{{100, 5}, {},        {23, 15}, {4, 30}},
			{{50, 9},  {15, 15},  {10, 5},  {9, 12}},
			{{20, 7},  {},        {12, 11}, {5, 18},  {3, 22}},
			{{},       {},        {20, 8},  {8, 17},  {4, 19}}
		};
};
