#include "game.h"

DrawableText::DrawableText (const char* message, int _x, int _y, SDL_Color color) {
    x = _x;
    y = _y;
    SDL_Surface* surface = TTF_RenderText_Solid(game_font, message, color);
    if (surface == NULL) {
        derror("TTF_RenderText_Solid");
        //TTF_RENDER
        return;
    }

    text_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (text_texture == NULL) {
        derror("SDL_CreateTextureFromSurface");
        return;
    }

    // Cleanup
    SDL_FreeSurface(surface);
}

DrawableText::~DrawableText () {
    SDL_DestroyTexture(text_texture);
}

void DrawableText::draw () {
    int w, h;
    SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);

    SDL_Rect dest;
    dest.w = w;
    dest.h = h;
    dest.x = x;
    dest.y = y;

    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
}

