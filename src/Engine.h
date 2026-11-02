#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "GameObject.h"

using json = nlohmann::json;

class Engine {
public:
    static int width, height;
    static float dt;
    static SDL_Renderer* renderer;

    static std::vector<std::unique_ptr<GameObject>> objects;

    // Level state shown in the window title
    static int collected, required, lives;
    static bool won;

    static bool init(const char* title, int w, int h);
    static void run();

    // Factory entry points (JSON -> GameObject)
    static void loadPrefabs(const std::string& path);
    static std::unique_ptr<GameObject> create(const std::string& prefab, const json& overrides);
    static void spawn(const std::string& prefab, const json& overrides);

    static GameObject* find(const std::string& tag);
    static void requestLevel(const std::string& path);
    static void restartLevel() { requestLevel(currentLevel); }
    static void finishLevel();

    static std::string loadProgress();

private:
    static void loadLevel(const std::string& path);
    static void update();
    static void render();

    static SDL_Window* window;
    static bool running;
    static json prefabs;
    static std::vector<std::unique_ptr<GameObject>> pending;
    static std::string currentLevel, nextLevel, levelName, requestedLevel, title;
    static SDL_Color background;
};
