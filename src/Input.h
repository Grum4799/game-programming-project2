#pragma once
#include <SDL2/SDL.h>
#include <set>


class Input {
public:
    static void processEvent(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) keysDown.insert(event.key.keysym.sym);
        if (event.type == SDL_KEYUP)   keysDown.erase(event.key.keysym.sym);
    }

    static bool isKeyDown(SDL_Keycode key) { return keysDown.count(key) > 0; }

private:
    static std::set<SDL_Keycode> keysDown;
};
