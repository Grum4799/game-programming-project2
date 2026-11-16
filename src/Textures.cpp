#include "Textures.h"
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include "Engine.h"

std::unordered_map<std::string, SDL_Texture*> Textures::textures;

bool Textures::load(const std::string& manifestPath) {
    std::ifstream file(manifestPath);
    if (!file) { std::cerr << "[Textures] Could not open " << manifestPath << "\n"; return false; }
    json manifest = json::parse(file);

    for (auto& [key, path] : manifest.at("textures").items()) {
        std::string p = path.get<std::string>();
        SDL_Texture* tex = IMG_LoadTexture(Engine::renderer, p.c_str());
        if (!tex) { SDL_Log("Failed to load %s: %s", p.c_str(), IMG_GetError()); continue; }
        textures[key] = tex;
    }
    std::cout << "[Textures] Loaded " << textures.size() << " textures from " << manifestPath << "\n";
    return true;
}

SDL_Texture* Textures::get(const std::string& key) {
    auto it = textures.find(key);
    if (it != textures.end()) return it->second;
    SDL_Log("Texture not found: %s", key.c_str());
    return nullptr;
}

void Textures::clear() {
    for (auto& pair : textures)
        if (pair.second) SDL_DestroyTexture(pair.second);
    textures.clear();
}
