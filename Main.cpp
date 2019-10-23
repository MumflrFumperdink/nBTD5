#include <os.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <cmath>

#include "utils.h"
#include "Interface/Mouse.h"
#include "Interface/HUD.h"
#include "Interface/Level.h"

SDL_Surface* screen;
nSDL_Font* font;
nSDL_Font* grey_font;
nSDL_Font* black_font;
bool done = false;
unsigned long oT = 0;
Mouse m;
std::vector<std::array<short, 2>> checkpoints;
std::vector<GameObject> interactables;
std::vector<unsigned char> underneathCheckpoints;
std::vector<SDL_Rect> trackCheckpoints;

void init() {
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);
    if (screen == NULL) {
        printf("Couldn't initialize display: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_ShowCursor(SDL_DISABLE);

    printf("Initialized SDL and display.\n");

    //Set up gameObjects
    for (unsigned char i = 0; i < 7; i++) {
        interactables.push_back({CHARACHTER_SELECT, {70 + (i * 35), 220, 18, 18}});
    }
    interactables.push_back({UPGRADE_MENU, {233, 80, 87, 45}}); //Upgrade 1
    interactables.push_back({UPGRADE_MENU, {233, 130, 87, 45}}); //Upgrade 2
    interactables.push_back({SPEED, {5, 185, 25, 25}}); //Speed select
}

void quit() {
    SDL_FreeSurface(screen);
    nSDL_FreeFont(font);
    nSDL_FreeFont(grey_font);
    nSDL_FreeFont(black_font);
    interactables.clear();
    interactables.shrink_to_fit();
    checkpoints.clear();
    checkpoints.shrink_to_fit();
    trackCheckpoints.clear();
    trackCheckpoints.shrink_to_fit();
    m.quit();
    SDL_Quit();
}

int main(void) {
    init();

    font = nSDL_LoadFont(NSDL_FONT_VGA, 250, 251, 252);
    grey_font = nSDL_LoadFont(NSDL_FONT_VGA, 131, 127, 122);
    black_font = nSDL_LoadFont(NSDL_FONT_VGA, 0, 0, 0);

    {
        m = Mouse();
        HUD h = HUD();
        Level level = Level(MONKEY_LANE);

        long tp1 = 0;
        long tp2 = 0;
        float fElapsedTime = 0.0;

        while (!done) {
            // Get time in seconds (GetTicks() gets it in milliseconds)
            tp2 = SDL_GetTicks();
            fElapsedTime = (tp2 - tp1) / 1000.0;
            tp1 = tp2;

            level.draw(&h);
            level.loop(fElapsedTime, &h);

            h.draw(fElapsedTime);

            m.update();
            if (m.hasMoved()) {
                //Moved
            }
            if (m.hasClicked()) {
                if (m.isTouchingInteractable() && ((h.getSelected() < 0) || (h.getSelected() > 9 && (m.whichInteractable(&h) == 7 || m.whichInteractable(&h) == 8)))) {
                    h.select(m.whichInteractable(&h), &level);
                }
                else {
                    if (m.getY() < LEVEL_HEIGHT && h.getSelected() >= 0 && h.getSelected() < 7) {
                        level.addCharachter(&m, &h);
                    }
                    h.deselect(&level);
                }
            }
            m.draw(&h);

            if (isKeyPressed(KEY_NSPIRE_ESC)) {
                done = true;
            }

            oT++;

            SDL_Flip(screen);
        }
    }

    quit();
    exit(EXIT_SUCCESS);
}
