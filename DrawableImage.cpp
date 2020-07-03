#include "game.h"

DrawableImage::DrawableImage (unsigned int texture, int _x, int _y) {
    image_texture = game_images[texture];
    x = _x;
    y = _y;
}

void DrawableImage::draw () {
    int w, h;
    SDL_QueryTexture(image_texture, NULL, NULL, &w, &h);

    SDL_Rect dest;
    dest.w = w;
    dest.h = h;
    dest.x = x;
    dest.y = y;

    SDL_RenderCopy(renderer, image_texture, NULL, &dest);
}
