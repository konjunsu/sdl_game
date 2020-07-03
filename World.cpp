#include "game.h"

World::World (std::vector<GameObject*> _objects) {
    objects = _objects;
}

void World::checkMouseOver (SDL_Point* mousepos) {
    SDL_Rect rect;
    for (GameObject* go : objects) {
        rect = go->getRect();
        if (SDL_PointInRect(mousepos, &rect)) {
            go->onMouseOver();
        }
    }
}

bool World::checkMouseClick (SDL_Point* mousepos, Uint8 mouse_button) {
    SDL_Rect rect;
    for (GameObject* go : objects) {
        rect = go->getRect();
        if (SDL_PointInRect(mousepos, &rect)) {
            go->onMouseDown(mouse_button);
            return true;
        }
    }
}

void World::drawWorld () {
    for (GameObject* go : objects) {
        go->draw();
    }
}

