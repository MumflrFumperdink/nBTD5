#include "Character.h"

Character::Character(short inx, short iny) {
  x = inx;
  y = iny;
  lefty = x < S_WIDTH / 2;
  range = angle = 0;
  selected = false;
  targeting = -1;
  stage = 1;
  timeDifference = 0.0;
}

Character::~Character() {
  //Some Stuff
}

void Character::select() {
  hud_x = 0;
  selected = true;

  if (lefty) {
    interactables[7].pos.x = 233;
    interactables[8].pos.x = 233;
  }
  else {
    interactables[7].pos.x = 1;
    interactables[8].pos.x = 1;
  }
}

void Character::deselect() {
  selected = false;
}

#define FULL_UPGRADE_BAR 91

void Character::upgradeSideBar(const char* title, SDL_Surface* upgrade1, unsigned char addx, char* title1, SDL_Surface* upgrade2, char* title2, SDL_Surface* avatar, bool* avail) const {
  if (hud_x < FULL_UPGRADE_BAR) {
    hud_x += 13;
  }
  else hud_x = FULL_UPGRADE_BAR;

  SDL_Surface* upgrade_back = nSDL_LoadImage(hud_upgrade_sidebar);
  if (x >= S_WIDTH / 2) {
    SDL_Surface* temp = flip_surface(upgrade_back, FLIP_HORIZONTAL);
    SDL_FreeSurface(upgrade_back);
    upgrade_back = temp;
  }
  SDL_SetColorKey(upgrade_back, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade_back->format, 0, 0, 0));
  SDL_CustomBlitCorner(upgrade_back, lefty ? S_WIDTH - hud_x : hud_x - FULL_UPGRADE_BAR, 0);
  SDL_FreeSurface(upgrade_back);

  SDL_Surface* upgrade_title = nSDL_LoadImage(hud_upgrade_title_back);
  SDL_SetColorKey(upgrade_title, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(upgrade_title->format, 0, 0, 0));
  SDL_CustomBlitCorner(upgrade_title, lefty ? S_WIDTH - (hud_x * 7 / 8) : hud_x + 7 - FULL_UPGRADE_BAR, 5);
  SDL_FreeSurface(upgrade_title);

  if ((hud_x + 8 - FULL_UPGRADE_BAR >= 0 && lefty) || !lefty) nSDL_DrawString(screen, font, lefty ? S_WIDTH - (hud_x * 6 / 7) : hud_x + 8 - FULL_UPGRADE_BAR, 10, title);

  SDL_Rect border = {lefty ? S_WIDTH - hud_x + 8 : hud_x + 8 - FULL_UPGRADE_BAR, 31, 42, 42};
  SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 72, 42, 2));

  SDL_CustomBlitCorner(avatar, lefty ? S_WIDTH - (hud_x * 7 / 8) : hud_x + 11 - FULL_UPGRADE_BAR, 35);

  signed short xpos = lefty ? S_WIDTH - hud_x + 5 : hud_x + 1 - FULL_UPGRADE_BAR;
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
