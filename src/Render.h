#ifndef RENDER_H
#define RENDER_H

#include "Globals.h"

SDL_Texture* loadTexture(const char* filePath);
SDL_Texture* renderText(const char* text, SDL_Color color);
void renderMenu();
void renderStore();
void render();
void renderHighScore();

#endif // RENDER_H