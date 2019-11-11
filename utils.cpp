#include "utils.h"

double radians(double degree) {
  return degree * PI / 180.0;
}

double degrees(double radian) {
  return radian * 180.0 / PI;
}

double distBetween(short x1, short y1, short x2, short y2) {
  short sideH = abs(x1 - x2);
  short sideV = abs(y1 - y2);
  return hypot(sideH, sideV);
}

double angleBetween(short x1, short y1, short x2, short y2) {
  short sideH = x1 - x2;
  short sideV = y1 - y2;

  return 90 - degrees(std::atan2(sideV, sideH));
}

//From http://sdl.beuc.net/sdl.wiki/Pixel_Access
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
    case 1:
      return *p;
      break;

    case 2:
      return *(Uint16 *)p;
      break;

    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
      break;

    case 4:
      return *(Uint32 *)p;
      break;

    default:
      return 0;       /* shouldn't happen, but avoids warnings */
  }
}

//http://www.cs.cmu.edu/afs/cs/user/meogata/Scramble/W%20i%20i/SDL_gfx/Docs/
SDL_Surface* flip_surface( SDL_Surface *surface, int flags) {
  //Pointer to the soon to be flipped surface
  SDL_Surface* flipped = nullptr;

  //If the image is color keyed
  if( surface->flags & SDL_SRCCOLORKEY )
  {
      flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
  }
  //Otherwise
  else
  {
      flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
  }

  //If the surface must be locked
  if( SDL_MUSTLOCK( surface ) )
  {
      //Lock the surface
      SDL_LockSurface( surface );
  }

  //Go through columns
  for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
  {
      //Go through rows
      for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
      {
          //Get pixel
          Uint32 pixel = get_pixel(surface, x, y);

          //Copy pixel
          if( ( flags & FLIP_VERTICAL ) && ( flags & FLIP_HORIZONTAL ) )
          {
              put_pixel(flipped, rx, ry, pixel);
          }
          else if( flags & FLIP_HORIZONTAL )
          {
              put_pixel(flipped, rx, y, pixel);
          }
          else if( flags & FLIP_VERTICAL )
          {
              put_pixel(flipped, x, ry, pixel);
          }
      }
  }

  //Unlock surface
  if( SDL_MUSTLOCK( surface ) )
  {
      SDL_UnlockSurface( surface );
  }

  //Copy color key
  if( surface->flags & SDL_SRCCOLORKEY )
  {
      SDL_SetColorKey( flipped, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
  }

  //Return flipped surface
  return flipped;
}
// </Copying and pasting from lazyfoo.net>

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

void SDL_CustomBlit(SDL_Surface* img, float x, float y, double angle) {
  SDL_CustomBlit(img, (short)x, (short)y, angle);
}

void SDL_CustomBlitCorner(SDL_Surface* img, short x, short y) {
  SDL_Rect img_rect = {x, y};
  SDL_BlitSurface(img, NULL, screen, &img_rect);
}

void SDL_RotateBlitAroundPoint(SDL_Surface* img, double angle, short dist, short x, short y) { //x and y are center of rotation
  SDL_CustomBlit(img, (short)(x + (short)(dist * sin(radians(angle + 90.0)))), (short)(y + (short)(dist * cos(radians(angle + 90.0)))), angle);
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
