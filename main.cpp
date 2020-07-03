#include "game.h"

const Uint8* keyboard_state = NULL;

void derror (const char* err_name) {
    printf ("ERROR: %s failed: %s\n", err_name, SDL_GetError());
    exit(1);
}

SDL_Renderer* renderer = NULL;
TTF_Font* game_font = NULL;

std::vector<SDL_Texture*> game_images;

const char *image_files[] = {
    "map.png",
    "sl.png",
    "ys.png",
    "ac.png",
    "sv.png"
};

bool load_image_file_index (unsigned int index) {
    SDL_Surface* image = IMG_Load(image_files[index]);
    if (image == NULL) {
        return false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == NULL) {
        SDL_FreeSurface(image);
        return false;
    }
    game_images.push_back(texture);
    return true;
}

void flag_select_callback (void* param1, void* param2) {
    switch ((unsigned long long) param1)
    {
    case 0:
        printf ("SLO!\n");
        break;
    case 1:
        printf ("SR SLO!\n");
        break;
    case 2:
        printf ("AV CES!\n");
        break;
    case 3:
        printf ("SV RIM CES!\n");
        break;
    default:
        printf ("wtf si naredu\n");
        break;
    }
}


bool isWindowMinimized = false;
bool isWindowFullscreen = false;

int main () {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        derror("SDL_Init");
    }

    if (TTF_Init() != 0) {
        derror("TTF_Init");
    }

    printf ("Loading font 04b-03.ttf ...\n");
    game_font = TTF_OpenFont("04b-03.ttf", 24);

    printf ("Getting SDL keyboard states ...\n");
    keyboard_state = SDL_GetKeyboardState(NULL);

    // Window creation
    SDL_Window* window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

//    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    printf ("Loading images ...\n");
    for (int i=0; i<SIZE(image_files); i++) {
        printf ("%s ...\t\t", image_files[i]);
        if (load_image_file_index(i)) printf("OK\n");
        else printf("FAIL (%s)\n", SDL_GetError());
    }

    ButtonImage* slovenija = new ButtonImage(1, 100, 75, flag_select_callback, (void*)0);
    ButtonImage* sr_slovenija = new ButtonImage(2, 100, 200, flag_select_callback, (void*)1);
    ButtonImage* av_cesarstvo = new ButtonImage(3, 100, 325, flag_select_callback, (void*)2);
    ButtonImage* sv_cesarstvo = new ButtonImage(4, 100, 450, flag_select_callback, (void*)3);
    DrawableText* txt1 = new DrawableText("Slovenija", 400, 113, {255, 255, 255, 255});
    DrawableText* txt2 = new DrawableText("SR Slovenija", 400, 238, {255, 255, 255, 255});
    DrawableText* txt3 = new DrawableText("Avstrijsko Cesarstvo", 400, 363, {255, 255, 255, 255});
    DrawableText* txt4 = new DrawableText("Sveto Rimsko Cesarstvo", 400, 488, {255, 255, 255, 255});
    DrawableText* drzava = new DrawableText("Izberi Drzavo", 200, 10, {255, 255, 255, 255});


    World* world = new World({slovenija, sr_slovenija, av_cesarstvo, sv_cesarstvo, txt1, txt2, txt3, txt4, drzava});

    Uint64 timeNow, timeLast = 0;
    double deltaTime = 0;

    timeNow = SDL_GetPerformanceCounter();
    //DrawableText* dtxt = new DrawableText("Testing SDL text (TTF)", 0, 0, {255, 255, 255, 255});
    DrawableImage* gamemap = new DrawableImage(0, 0, 0);
    DrawableText* fps = new DrawableText("60 FPS", 0, 0, {255, 255, 255, 255});
    std::vector<double> measure;

    char chFPS[32];
    chFPS[31] = '\0';

    SDL_Point mouse_coords;
    SDL_Point click_point;
    int o_x = gamemap->x, o_y = gamemap->y; // original

    bool moving_map = false;
    bool draw_fps = false;

    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    bool close = false;
    SDL_Event event;
    while (!close) {
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                close = true;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                mouse_coords.x = event.motion.x;
                mouse_coords.y = event.motion.y;
                if (moving_map) {
		    //printf("Mousemotion\n");
                    gamemap->x = o_x + (mouse_coords.x - click_point.x);
                    gamemap->y = o_y + (mouse_coords.y - click_point.y);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (!moving_map && !world->checkMouseClick(&mouse_coords, event.button.button) && event.button.button == SDL_BUTTON_LEFT) {
                    //printf("Mousedown!\n");
		            click_point = mouse_coords;
                    moving_map = true;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT && moving_map) {
                    moving_map = false;
                    o_x = gamemap->x;
                    o_y = gamemap->y;
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.scancode == SDL_SCANCODE_F3) {
                    draw_fps = !draw_fps;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_F10) {
                    isWindowFullscreen = !isWindowFullscreen;
                    if (isWindowFullscreen) SDL_SetWindowFullscreen (window, SDL_WINDOW_FULLSCREEN);
                    else SDL_SetWindowFullscreen(window, 0);
                }
            }
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) {
                    isWindowMinimized = true;
                }
                else if (event.window.event == SDL_WINDOWEVENT_RESTORED) {
                    isWindowMinimized = false;
                }
            }
	    }

        world->checkMouseOver(&mouse_coords);

        // delta
        timeLast = timeNow;
        timeNow = SDL_GetPerformanceCounter();
        deltaTime = (double)((timeNow - timeLast)*0.001 / (double)SDL_GetPerformanceFrequency());

        // FPS
        measure.push_back(1.0/(deltaTime*1000.0));
        if (measure.size() >= 20) {
            double avg = 0;
            for (double d : measure) {
                avg += d;
            }
            avg /= (double) measure.size();
            measure.clear();
            sprintf (chFPS, "%.2f FPS", avg);
            delete fps;
            fps = new DrawableText(chFPS, 0, 0, {255, 255, 255, 255});
        }


        SDL_RenderClear (renderer);
        //printf("Drawing world ...\n");
        //printf("Drawing text ...\n");
        gamemap->draw();
        world->drawWorld();
        //dtxt->draw();
        if (draw_fps)
	        fps->draw();
        //printf("Done!\n");
        SDL_RenderPresent (renderer);
        if (isWindowMinimized)
            SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
