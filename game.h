#ifndef GAME
#define GAME

#define SDL_MAIN_HANDLED
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SIZE(x) (sizeof(x)/sizeof(*x))

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>

// Basic class. some neccessary methods and events
class GameObject {
    public:
        virtual void draw () = 0;
        virtual void update (float delta) = 0;
        virtual void handleInput (SDL_Scancode scancode) {}
        virtual SDL_Rect getRect () {}
        virtual void onMouseDown (Uint8 mouse_button) {}
        virtual void onMouseOver () {}
};

class DrawableText {
    private:
        SDL_Texture* text_texture;
    public:
        int x;
        int y;
        void draw ();
        DrawableText(const char* message, int _x, int _y, SDL_Color color);
        ~DrawableText();
};

class DrawableImage {
    private:
        SDL_Texture* image_texture;
    public:
        int x;
        int y;
        void draw ();
        DrawableImage(unsigned int texture, int _x, int _y);
};

class ButtonImage : public GameObject {
    
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

#endif