#ifndef GAME
#define GAME

#define SDL_MAIN_HANDLED
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SIZE(x) (sizeof(x)/sizeof(*x))

typedef void (*GAME_CALLBACK)(void*, void*);

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <wchar.h>

class GameObject {
    public:
        virtual void draw () = 0;
        virtual void update (float delta) = 0;
        virtual void handleInput (SDL_Scancode scancode) {}
        virtual SDL_Rect getRect () {}
        virtual void onMouseDown (Uint8 mouse_button) {}
        virtual void onMouseOver () {}
};

class DrawableText : public GameObject {
    private:
        SDL_Texture* text_texture;
    public:
        int x;
        int y;
        virtual void draw ();
        virtual void update (float delta) {}
        DrawableText(const char* message, int _x, int _y, SDL_Color color);
        ~DrawableText();
};

class DrawableImage : public GameObject {
    private:
        SDL_Texture* image_texture;
    public:
        int x;
        int y;
        virtual void draw ();
        virtual void update (float delta) {}
        SDL_Rect getImageRect ();
        DrawableImage(unsigned int texture, int _x, int _y);
};

class ButtonImage : public GameObject {
    private:
        DrawableImage* image;
        SDL_Rect image_rect;
        bool isMouseOver;
        GAME_CALLBACK on_click;
        void* param;

    public:
        ButtonImage(unsigned int texture, int _x, int _y, GAME_CALLBACK _on_click, void* _param);
        virtual void draw ();
        virtual void update (float delta) {}
        virtual void onMouseOver ();
        virtual void onMouseDown (Uint8 mouse_button);
        virtual SDL_Rect getRect ();
};

class World {
    private:
        std::vector<GameObject*> objects;

    public:
        World (std::vector<GameObject*> _objects);
        void drawWorld ();
        void checkMouseOver (SDL_Point* mousepos);
        bool checkMouseClick (SDL_Point* mousepos, Uint8 mouse_button);
};

void derror(const char* err_name);

extern SDL_Renderer* renderer;
extern const Uint8* keyboard_state;
extern TTF_Font* game_font;
extern std::vector<SDL_Texture*> game_images;

void flag_select_callback (void* param1, void* param2);

#endif