#pragma once

#define S_WIDTH 320
#define S_HEIGHT 240

#define LEVEL_HEIGHT 212
#define HUD_HEIGHT 38

#define PI 3.14159265

#include "images.h"

#include <os.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <vector>
#include <cmath>
#include <array>

enum LevelName {
	MONKEY_LANE //0
};

enum Charachter {
	DART_MONKEY, //0
  TACK_SHOOTER,
  SNIPER_MONKEY,
  BOOMERANG_MONKEY
};

enum BloonType {
	RED,         //0
	BLUE,        //1
	GREEN,       //2
  YELLOW,      //3
  PINK,        //4
	numBloonTypes
};

enum Type {
	CHARACHTER_SELECT, //0
	SPEED,			       //1
	UPGRADE_MENU,      //2
	CHARACHTER         //3
};

struct GameObject {
	Type type;
	SDL_Rect pos;
};

//Global variables that all classes should have access to
extern unsigned long oT;
extern bool done;
extern SDL_Surface* screen;
extern nSDL_Font *font, *grey_font, *black_font;
extern std::vector<GameObject> interactables; //0-6 are charachter_select at bottom, then 7-8 are upgrade, 9 is speed, rest are charachters
extern std::vector<std::array<short, 2>> checkpoints;
extern std::vector<SDL_Rect> trackCheckpoints;

static const touchpad_info_t *tpad_info = touchpad_getinfo();

//initial range of charachters
static const short charRange[] = {40, 0, 0, 60};
//cost of charachters
static const short charCost[] = {200, 0, 0, 380};
//health bloons take away
static const short bloonHealth[] = {1, 2, 3, 4, 5};
//bloons speeds
static const float bloonSpeed[] = {1, 1.33, 1.66, 3.33, 3.66};
//bloon worth in money
static const unsigned int bloonWorth[] = {1, 2, 3, 4, 5};
//upgrade pricess per charchter
static const unsigned short Dart_Monkey_upgradePrices[2][4] = {
  {90, 120, 500, 1500},
  {140, 170, 330, 8000}
};
static const unsigned short Boomerang_Monkey_upgradePrices[2][4] = {
  {250, 280, 1400, 9000},
  {100, 150, 1600, 3000}
};

double radians(double degrees);
void SDL_CustomBlit(SDL_Surface* img, short x, short y, double angle);
void SDL_CustomBlitCorner(SDL_Surface* img, short x, short y);
void SDL_RotateBlitAroundPoint(SDL_Surface* img, double angle, short dist, short x, short y);
void SDL_RotateBlitAroundPoint(SDL_Surface* img, double angle, short dist, short x, short y, short offsetY);
bool SDL_IntersectRect(SDL_Rect *A, SDL_Rect *B, SDL_Rect *intersection);
double distBetween(short x1, short y1, short x2, short y2);
double angleBetween(short x1, short y1, short x2, short y2);
SDL_Surface* load_Level(LevelName choice);
SDL_Surface* load_Charachter(Charachter choice);
SDL_Surface* load_Bloon(BloonType choice);
