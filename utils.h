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

typedef enum {
	MONKEY_LANE //0
} LevelName;

typedef enum {
	DART_MONKEY, //0
  TACK_SHOOTER,
  SNIPER_MONKEY,
  BOOMERANG_MONKEY
} Charachter;

typedef enum {
	RED,         //0
	BLUE,        //1
	GREEN,       //2
  YELLOW,      //3
  PINK,        //4
	numBloonTypes
} BloonType;

typedef enum {
	CHARACHTER_SELECT, //0
	SPEED,			       //1
	UPGRADE_MENU,      //2
	CHARACHTER         //3
} Type;

typedef struct {
	Type type;
	SDL_Rect pos;
} GameObject;

#include <algorithm>
#include <initializer_list>

template <typename T, std::size_t N>
struct Array {
    T body[N];

    Array(std::initializer_list<T> const& list) {
      std::copy(list.begin(), list.end(), body);
    }

    operator T* () { return body; }
    constexpr std::size_t size() const { return N; }

    T operator [](std::size_t n) const {
      return body[n];
    }

    T* begin() const { return body; }

    T* end()   const { return body + N; }

    T& at(std::size_t offset) const {
      return body[offset];
    }
};

//Global variables that all classes should have access to
extern unsigned long oT;
extern bool done;
extern SDL_Surface* screen;
extern nSDL_Font *font, *grey_font, *black_font;
extern std::vector<GameObject> interactables; //0-6 are charachter_select at bottom, then 7-8 are upgrade, 9 is speed, rest are charachters
extern std::vector<Array<short, 2>> checkpoints;
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

static double radians(double degrees) {
	return degrees * PI / 180.0;
}

static void SDL_CustomBlit(SDL_Surface* img, short x, short y, double angle) { //x and y are center of image
	if (angle == 0.0) {
		SDL_Rect img_rect = {x - (img->w/2), y - (img->h/2)};
		SDL_BlitSurface(img, NULL, screen, &img_rect);
	}
	else {
		SDL_Surface* rotated = rotozoomSurface(img, angle, 1, SMOOTHING_ON);
		SDL_SetAlpha(rotated, false, SDL_ALPHA_OPAQUE); //rotozoomSurface() returns a surface that has alpha, turn off to set colorkey (transparency)
		SDL_SetColorKey(rotated, SDL_SRCCOLORKEY | SDL_RLEACCEL, img->format->colorkey);
		SDL_Rect img_rect = {x - (rotated->w/2), y - (rotated->h/2)};
		SDL_BlitSurface(rotated, NULL, screen, &img_rect);
		SDL_FreeSurface(rotated);
	}
}

static void SDL_CustomBlitCorner(SDL_Surface* img, short x, short y) {
	SDL_Rect img_rect = {x, y};
	SDL_BlitSurface(img, NULL, screen, &img_rect);
}

static void SDL_RotateBlitAroundPoint(SDL_Surface* img, double angle, short dist, short x, short y) { //x and y are center of rotation
	SDL_CustomBlit(img, x + (short)(dist * sin(radians(angle + 90.0))), y + (short)(dist * cos(radians(angle + 90.0))), angle);
}

static void SDL_RotateBlitAroundPoint(SDL_Surface* img, double angle, short dist, short x, short y, short offsetY) {
	SDL_RotateBlitAroundPoint(img, angle, dist, x - (short)(offsetY * cos(radians(angle + 90.0))), y + (short)(offsetY * sin(radians(angle + 90.0))));
}


static bool SDL_IntersectRect(SDL_Rect *A, SDL_Rect *B, SDL_Rect *intersection) {
	short Amin, Amax, Bmin, Bmax;

	/* Horizontal intersection */
	Amin = A->x;
	Amax = Amin + A->w;
	Bmin = B->x;
	Bmax = Bmin + B->w;
	if(Bmin > Amin) {
	    Amin = Bmin;
	}
	intersection->x = Amin;
	if (Bmax < Amax) {
	    Amax = Bmax;
	}
	intersection->w = Amax - Amin > 0 ? Amax - Amin : 0;

	/* Vertical intersection */
	Amin = A->y;
	Amax = Amin + A->h;
	Bmin = B->y;
	Bmax = Bmin + B->h;
	if(Bmin > Amin) {
	    Amin = Bmin;
	}
	intersection->y = Amin;
	if(Bmax < Amax) {
	    Amax = Bmax;
	}
	intersection->h = Amax - Amin > 0 ? Amax - Amin : 0;

	return (intersection->w && intersection->h);
} //Copied from https://github.com/hoffa/nSDL/blob/de58382bd540c26cbd614259541f967277662b72/src/video/SDL_surface.c

static double distBetween(short x1, short y1, short x2, short y2) {
	short sideH = abs(x1 - x2);
  short sideV = abs(y1 - y2);
  return hypot(sideH, sideV);
}

static double angleBetween(short x1, short y1, short x2, short y2) {
  short sideH = x1 - x2;
  short sideV = y1 - y2;

  return 90 - std::atan2(sideV, sideH) * 180 / PI;
}

//Functions with Images
static SDL_Surface* load_Level(LevelName choice) {
  SDL_Surface* result;
  switch (choice) {
    case MONKEY_LANE:
      result = nSDL_LoadImage(monkey_lane_back);
      break;
    default:
      result = nSDL_LoadImage(null_Surface);
      break;
  }
  SDL_SetColorKey(result, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(result->format, 0, 0, 0));
  return result;
}

static SDL_Surface* load_Charachter(Charachter choice) {
	SDL_Surface* result;
	switch (choice) {
		case DART_MONKEY: result = nSDL_LoadImage(charachter_dartMonkey); break;
    case BOOMERANG_MONKEY: result = nSDL_LoadImage(charachter_boomerangMonkey); break;
		default: result = nSDL_LoadImage(null_Surface); break;
	}
	SDL_SetColorKey(result, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(result->format, 0, 0, 0));
	return result;
}

static SDL_Surface* load_Bloon(BloonType choice) {
	SDL_Surface* result;
	switch (choice) {
		case RED: result = nSDL_LoadImage(red_bloon); break;
		case BLUE: result = nSDL_LoadImage(blue_bloon); break;
		case GREEN: result = nSDL_LoadImage(green_bloon); break;
    case YELLOW: result = nSDL_LoadImage(yellow_bloon); break;
    case PINK: result = nSDL_LoadImage(pink_bloon); break;
		default: result = nSDL_LoadImage(null_Surface); break;
	}
	SDL_SetColorKey(result, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(result->format, 0, 0, 0));
	return result;
}
