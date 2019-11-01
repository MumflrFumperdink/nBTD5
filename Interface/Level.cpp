#include <SDL/SDL.h>
#include <algorithm>

#include "Level.h"
#include "Mouse.h"
#include "HUD.h"
#include "../utils.h"
#include "../Charachters/Character.h"
#include "../Charachters/DartMonkey.h"
#include "../Charachters/BoomerangMonkey.h"
#include "../Other/Bloon.h"
#include "../Projectile/Projectile.h"
#include "../Projectile/Dart.h"

Level::Level(LevelName l) {
	timePassed = 0.0;
	name = l;
	switch (name) {
		case MONKEY_LANE:
			level = nSDL_LoadImage(monkey_lane_back);

			checkpoints.push_back({0, 118});
			checkpoints.push_back({75, 118});
			checkpoints.push_back({75, 84});
			checkpoints.push_back({120, 84});
			checkpoints.push_back({120, 195});
			checkpoints.push_back({190, 195});
			checkpoints.push_back({190, 185}); //before tunnel 1
			checkpoints.push_back({190, 115}); //after tunnel 1
			checkpoints.push_back({190, 51});
			checkpoints.push_back({190, 51});
			checkpoints.push_back({72, 51});
			checkpoints.push_back({72, 18});
			checkpoints.push_back({227, 18});
			checkpoints.push_back({227, 51});
			checkpoints.push_back({270, 51});
			checkpoints.push_back({270, 85});
			checkpoints.push_back({228, 85});
			checkpoints.push_back({228, 150});
			checkpoints.push_back({155, 150}); //before tunnel 2
			checkpoints.push_back({85, 150});  //after tunnel 2
			checkpoints.push_back({72, 150});
			checkpoints.push_back({72, 220});

			underneathCheckpoints.push_back(7);
			underneathCheckpoints.push_back(19);
			break;
	}

	for (unsigned char i = 0, j = 0; i < checkpoints.size() - 1; i++) {
		if (j == underneathCheckpoints.size() || i + 1 != underneathCheckpoints.at(j)) {
			auto a = checkpoints[i];
			auto b = checkpoints[i + 1];
			if (a[0] - b[0] == 0) {
				if (a[1] - b[1] < 0) trackCheckpoints.push_back({a[0] - 6, a[1], 12, b[1] - a[1]});
				else trackCheckpoints.push_back({a[0] - 6, a[1] - (a[1] - b[1]), 12, a[1] - b[1]});
			}
			else {
				if (a[0] - b[0] < 0) trackCheckpoints.push_back({a[0], a[1] - 6, b[0] - a[0], 12});
				else trackCheckpoints.push_back({a[0] - (a[0] - b[0]), a[1] - 6, a[0] - b[0], 12});
			}
		}
		else j++;
	}
}

void Level::draw() {
	SDL_BlitSurface(level, NULL, screen, NULL);

	if (isKeyPressed(KEY_NSPIRE_ENTER)) {
		for (unsigned char i = 0; i < trackCheckpoints.size(); i++) {
			SDL_FillRect(screen, &trackCheckpoints[i], SDL_MapRGB(screen->format, 255 - (i * (255/trackCheckpoints.size())), 0, i * (255/trackCheckpoints.size())));
		}
	}

	for (signed short i = projectiles.size() - 1; i >= 0; i--) {
		projectiles[i]->draw();
	}

	for (short i = 0; i < bloons.size(); i++) {
		if (!bloons[i]->isUnderneath()) bloons[i]->draw();
	}

	for (short i = 0; i < charchters.size(); i++) {
		charchters[i]->draw();
	}
}

void Level::drawUp(HUD* h) {
	for (short i = 0; i < charchters.size(); i++) {
		charchters[i]->drawUp(h);
	}
}

bool Level::hasRoundCleared(unsigned char currentRound) {
	unsigned char count = 0;
	for (unsigned short i = 0; i < sizeof(rounds[currentRound])/sizeof(rounds[currentRound][0]); i++) {
		if (rounds[currentRound][i][0] <= 0) count++;
	}
	return count == numBloonTypes;
}

constexpr const static float timeBetween = 0.072;

void Level::loop(float fElapsedTime, HUD* h) {
	if (h->getRound() < 16 and h->getSpeed() > PAUSE) {
		timePassed += fElapsedTime;
		for (unsigned char i = 0; i < sizeof(rounds[h->getRound()])/sizeof(rounds[h->getRound()][0]); i++) {
			if (rounds[h->getRound()][i][0] > 0 && timePassed >= timeBetween * rounds[h->getRound()][i][1] * h->getInvSpeed()) { //Is the type in this round
				timePassed -= timeBetween * rounds[h->getRound()][i][1] * h->getInvSpeed();
				bloons.push_back(new Bloon(static_cast<BloonType>(i)));
				rounds[h->getRound()][i][0]--;
			}
		}
	}

	for (signed short i = projectiles.size() - 1; i >= 0; i--) {
		if (projectiles[i]->gameLoop(fElapsedTime, &bloons, h) || projectiles[i]->getX() < 0 || projectiles[i]->getX() > S_WIDTH || projectiles[i]->getY() < 0 || projectiles[i]->getY() > S_HEIGHT) {
			delete projectiles[i];
			projectiles.erase(projectiles.begin() + i);
		}
	}

	for (short i = 0; i < charchters.size(); i++) {
		charchters[i]->gameLoop(fElapsedTime, h, &bloons, &projectiles);
	}

	for (short i = 0; i < bloons.size(); i++) {
		if (underneathCheckpoints.size() > 0) {
			if (!bloons[i]->isUnderneath() and std::find(underneathCheckpoints.begin(), underneathCheckpoints.end(), bloons[i]->getNextCheckpoint()) != underneathCheckpoints.end()) {//in Tunnel
				bloons[i]->goUnder();
			}
			else if (bloons[i]->isUnderneath() and std::find(underneathCheckpoints.begin(), underneathCheckpoints.end(), bloons[i]->getNextCheckpoint()) == underneathCheckpoints.end()) { //outOfTunnel
				bloons[i]->goOver();
			}
		}

		if (bloons[i]->gameLoop(fElapsedTime, h)) {
			if (bloons[i]->getNextCheckpoint() == checkpoints.size() - 1) { //reached the end
				h->setLives(h->getLives() - bloonHealth[static_cast<unsigned char>(bloons[i]->getType())]);
				if (h->getLives() <= 0) {
					show_msgbox("Game Over...", "You Lost! Better luck next time.");
					done = true;
				}
			}
			// else { //popped
			// 	h->setMoney(h->getMoney() - bloonWorth[static_cast<unsigned char>(bloons[i]->getType())]);
			// }

			delete bloons[i];
			bloons.erase(bloons.begin() + i);
		}
	}

	if (hasRoundCleared(h->getRound()) && (signed)bloons.size() <= 0) { //round ended
		for (auto &p : projectiles) delete p;
		projectiles.clear();
		timePassed = 0.0;
		h->setSpeed(PAUSE);
	}
}

Level::~Level() {
	SDL_FreeSurface(level);

	for (auto &a : charchters) delete a;
	charchters.clear();
	charchters.shrink_to_fit();
	for (auto &a : projectiles) delete a;
	projectiles.clear();
	projectiles.shrink_to_fit();
	for (auto &a : bloons) delete a;
	bloons.clear();
	underneathCheckpoints.clear();
	underneathCheckpoints.shrink_to_fit();
}

std::vector<Character*>& Level::getCharachters() {
	return charchters;
}

void Level::addCharachter(Mouse* m, HUD* h) {
	if (h->getMoney() >= charCost[h->getSelected()] && !m->intersects(&trackCheckpoints)) {
		h->addMoney(-charCost[h->getSelected()]);
		switch (static_cast<Charachter>(h->getSelected())) {
			case DART_MONKEY:
				charchters.push_back(new DartMonkey(m->getX(), m->getY()));
				interactables.push_back({CHARACHTER, {m->getX() - (charachter_dartMonkey[1] / 2), m->getY() - (charachter_dartMonkey[2] / 2), charachter_dartMonkey[1], charachter_dartMonkey[2]}});
				break;
			case BOOMERANG_MONKEY:
				charchters.push_back(new BoomerangMonkey(m->getX(), m->getY()));
				interactables.push_back({CHARACHTER, {m->getX() - (charachter_boomerangMonkey[1] / 2), m->getY() - (charachter_boomerangMonkey[2] / 2), charachter_boomerangMonkey[1], charachter_boomerangMonkey[2]}});
				break;
			default:;
		}
	}
}
