#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class Textures {
public:
    static SDL_Texture* get(const std::string& key);
    static void clear();

private:
    static std::unordered_map<std::string, SDL_Texture*> textures;
};
