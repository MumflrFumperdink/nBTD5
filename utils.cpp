#include "utils.h"

double radians(double degrees) {
  return degrees * PI / 180.0;
}

void SDL_CustomBlit(SDL_Surface* img, short x, short y, double angle) { //x and y are center of image
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

void SDL_CustomBlitCorner(SDL_Surface* img, short x, short y) {
  SDL_Rect img_rect = {x, y};
  SDL_BlitSurface(img, NULL, screen, &img_rect);
}

void SDL_RotateBlitAroundPoint(SDL_Surface* img, double angle, short dist, short x, short y) { //x and y are center of rotation
  SDL_CustomBlit(img, x + (short)(dist * sin(radians(angle + 90.0))), y + (short)(dist * cos(radians(angle + 90.0))), angle);
}

void SDL_RotateBlitAroundPoint(SDL_Surface* img, double angle, short dist, short x, short y, short offsetY) {
  SDL_RotateBlitAroundPoint(img, angle, dist, x - (short)(offsetY * cos(radians(angle + 90.0))), y + (short)(offsetY * sin(radians(angle + 90.0))));
}


bool SDL_IntersectRect(SDL_Rect *A, SDL_Rect *B, SDL_Rect *intersection) {
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

double distBetween(short x1, short y1, short x2, short y2) {
  short sideH = abs(x1 - x2);
  short sideV = abs(y1 - y2);
  return hypot(sideH, sideV);
}

double angleBetween(short x1, short y1, short x2, short y2) {
  short sideH = x1 - x2;
  short sideV = y1 - y2;

  return 90 - std::atan2(sideV, sideH) * 180 / PI;
}

//Functions with Images
SDL_Surface* load_Level(LevelName choice) {
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

SDL_Surface* load_Charachter(Charachter choice) {
  SDL_Surface* result;
  switch (choice) {
    case DART_MONKEY: result = nSDL_LoadImage(charachter_dartMonkey); break;
    case BOOMERANG_MONKEY: result = nSDL_LoadImage(charachter_boomerangMonkey); break;
    default: result = nSDL_LoadImage(null_Surface); break;
  }
  SDL_SetColorKey(result, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(result->format, 0, 0, 0));
  return result;
}

SDL_Surface* load_Bloon(BloonType choice) {
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
