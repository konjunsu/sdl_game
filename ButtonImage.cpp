#include "game.h"

/*
class ButtonImage : public GameObject {
    private:
        SDL_Texture* image_texture;
        SDL_Rect image_rect;

    public:
        ButtonImage(unsigned int texture, int _x, int _y);
        virtual void draw ();
        virtual void update (float delta) {}
        virtual void onMouseOver ();
};
*/


ButtonImage::ButtonImage (unsigned int texture, int _x, int _y, GAME_CALLBACK _on_click, void* _param) {
    image = new DrawableImage(texture, _x, _y);
    image_rect = image->getImageRect();
    isMouseOver = false;
    on_click = _on_click;
    param = _param;
}

void ButtonImage::onMouseOver () {
    //printf ("Mouse over!\n");
    isMouseOver = true;
}

void ButtonImage::draw () {
    if (isMouseOver) {
        isMouseOver = false;
        SDL_Rect rect;
        rect.x = image_rect.x-4;
        rect.y = image_rect.y-4;
        rect.w = image_rect.w+8;
        rect.h = image_rect.h+8;
        /*float sx, sy;
        SDL_RenderGetScale(renderer, &sx, &sy);
        SDL_RenderSetScale(renderer, 4.0f, 4.0f);*/
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderSetScale(renderer, sx, sy);
    }
    image->draw();
}

SDL_Rect ButtonImage::getRect () {
    return image_rect;
}

void ButtonImage::onMouseDown (Uint8 mouse_button) {
    if (mouse_button == SDL_BUTTON_LEFT) {
        on_click((void*) param, (void*) this);
    }
}
