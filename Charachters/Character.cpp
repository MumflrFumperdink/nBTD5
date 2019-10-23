#include "Character.h"

Character::Character(short inx, short iny) {
  x = inx;
  y = iny;
  range = angle = 0;
  selected = false;
  targeting = -1;
  stage = 1;
  timeDifference = 0.0;
}

Character::~Character() {
  //Some Stuff
}

void Character::upgradeSideBar(const char* title, SDL_Surface* upgrade1, unsigned char addx, char* title1, SDL_Surface* upgrade2, char* title2, bool* avail) {
  if (hud_x < 91) {
    hud_x += 10;
  }
  else hud_x = 91;

  SDL_Surface* upgrade_back = nSDL_LoadImage(hud_upgrade_sidebar);
  SDL_SetColorKey(upgrade_back, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade_back->format, 0, 0, 0));
  SDL_CustomBlitCorner(upgrade_back, S_WIDTH - hud_x, 0);
  SDL_FreeSurface(upgrade_back);

  SDL_Surface* upgrade_title = nSDL_LoadImage(hud_upgrade_title_back);
  SDL_SetColorKey(upgrade_title, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade_title->format, 0, 0, 0));
  SDL_CustomBlitCorner(upgrade_title, S_WIDTH - (hud_x * 7 / 8), 5);
  SDL_FreeSurface(upgrade_title);

  nSDL_DrawString(screen, font, S_WIDTH - (hud_x * 6 / 7), 10, title);

  SDL_Rect border = {S_WIDTH - hud_x + 8, 31, 42, 42};
  SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 72, 42, 2));

  short xpos = S_WIDTH - hud_x + 5;
  for (unsigned char i = 0; i < 2; i++) {
    SDL_Surface* upgrade_slot = avail[i] ? nSDL_LoadImage(hud_upgrade_slot) : nSDL_LoadImage(hud_upgrade_slot_unavailable);
    SDL_SetColorKey(upgrade_slot, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade_slot->format, 0, 0, 0));

    SDL_CustomBlitCorner(upgrade_slot, xpos, 80 + i * 50);
    SDL_CustomBlitCorner(i == 0 ? upgrade1 : upgrade2, xpos + 35 + (i == 0 ? addx : 0), 95 + i * 50);
    if (avail[i]) nSDL_DrawString(screen, black_font, xpos + 3, 85 + i * 50, i == 0 ? title1 : title2);
    nSDL_DrawString(screen, avail[i] ? font : grey_font, xpos + 4, 85 + i * 50, i == 0 ? title1 : title2);

    SDL_FreeSurface(upgrade_slot);
  }
}

unsigned char Character::hud_x = 0;
