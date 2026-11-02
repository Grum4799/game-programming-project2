#include "Textures.h"
#include <SDL2/SDL_image.h>
#include "Engine.h"

std::unordered_map<std::string, SDL_Texture*> Textures::textures;

SDL_Texture* Textures::get(const std::string& key) {
    auto it = textures.find(key);
    if (it != textures.end()) return it->second;

    std::string path = "assets/sprites/" + key + ".png";
    SDL_Texture* tex = IMG_LoadTexture(Engine::renderer, path.c_str());
    if (!tex) SDL_Log("Failed to load %s: %s", path.c_str(), IMG_GetError());
    textures[key] = tex;
    return tex;
}

void Textures::clear() {
    for (auto& pair : textures)
        if (pair.second) SDL_DestroyTexture(pair.second);
    textures.clear();
}
