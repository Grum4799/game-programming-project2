#include "Engine.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include "ComponentFactory.h"
#include "Input.h"
#include "Textures.h"

int Engine::width = 0, Engine::height = 0;
float Engine::dt = 0;
SDL_Renderer* Engine::renderer = nullptr;
SDL_Window* Engine::window = nullptr;
bool Engine::running = false;
std::vector<std::unique_ptr<GameObject>> Engine::objects;
std::vector<std::unique_ptr<GameObject>> Engine::pending;
json Engine::prefabs;
int Engine::collected = 0, Engine::required = 0, Engine::lives = 0;
bool Engine::won = false;
std::string Engine::currentLevel, Engine::nextLevel, Engine::levelName, Engine::requestedLevel, Engine::title;
SDL_Color Engine::background{20, 24, 36, 255};

static const char* SAVE_FILE = "save.json";

bool Engine::init(const char* t, int w, int h) {
    width = w; height = h;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { SDL_Log("SDL_Init: %s", SDL_GetError()); return false; }
    window = SDL_CreateWindow(t, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) : nullptr;
    if (!renderer) { SDL_Log("Window/renderer: %s", SDL_GetError()); return false; }
    running = true;
    return true;
}

void Engine::run() {
    Uint64 last = SDL_GetTicks64();
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) running = false;
            Input::processEvent(e);
        }
        Uint64 now = SDL_GetTicks64();
        dt = std::min((now - last) / 1000.0f, 1.0f / 30.0f);
        last = now;

        update();
        render();
        SDL_Delay(16);
    }
    objects.clear();
    pending.clear();
    Textures::clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::update() {
    for (size_t i = 0; i < objects.size(); ++i) objects[i]->update();

    for (auto& p : pending) objects.push_back(std::move(p));
    pending.clear();
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                                 [](const std::unique_ptr<GameObject>& o) { return !o->alive; }),
                  objects.end());

    if (!requestedLevel.empty()) {
        std::string path = requestedLevel;
        requestedLevel.clear();
        loadLevel(path);
    }

    std::string t = won ? "Power Runner | POWER RESTORED - you win!"
                        : "Power Runner | " + levelName + " | Lives: " + std::to_string(lives) +
                          " | Power: " + std::to_string(collected) + "/" + std::to_string(required);
    if (t != title) { title = t; SDL_SetWindowTitle(window, t.c_str()); }
}

void Engine::render() {
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, 255);
    SDL_RenderClear(renderer);
    for (auto& o : objects) o->draw();
    SDL_RenderPresent(renderer);
}

void Engine::loadPrefabs(const std::string& path) {
    std::ifstream file(path);
    if (!file) { std::cerr << "[Engine] Could not open " << path << "\n"; return; }
    prefabs = json::parse(file);
}

std::unique_ptr<GameObject> Engine::create(const std::string& prefab, const json& overrides) {
    auto obj = std::make_unique<GameObject>();
    const json& def = prefabs.at(prefab);
    obj->tag = def.value("tag", "");
    for (json component : def.at("components")) {
        std::string type = component.at("type");
        if (overrides.contains(type)) component.merge_patch(overrides[type]);
        ComponentFactory::add(*obj, component);
    }
    obj->initialize();
    return obj;
}

void Engine::spawn(const std::string& prefab, const json& overrides) {
    pending.push_back(create(prefab, overrides));
}

GameObject* Engine::find(const std::string& tag) {
    for (auto& o : objects)
        if (o->alive && o->tag == tag) return o.get();
    return nullptr;
}

void Engine::requestLevel(const std::string& path) { requestedLevel = path; }

void Engine::loadLevel(const std::string& path) {
    std::ifstream file(path);
    if (!file) { std::cerr << "[Engine] Could not open " << path << "\n"; running = false; return; }
    json level = json::parse(file);

    objects.clear();
    pending.clear();
    collected = 0;
    won = false;
    currentLevel = path;
    levelName = level.value("name", path);
    nextLevel = level.value("next", "");
    required = level.value("required", 0);
    if (level.contains("background")) {
        auto bg = level["background"];
        background = {Uint8(bg[0]), Uint8(bg[1]), Uint8(bg[2]), 255};
    }

    for (const json& entry : level.at("objects"))
        objects.push_back(create(entry.at("prefab"), entry.value("set", json::object())));

    std::ofstream(SAVE_FILE) << json{{"level", path}}.dump(2);
    std::cout << "[Engine] Loaded " << objects.size() << " objects from " << path << "\n";
}

void Engine::finishLevel() {
    if (!nextLevel.empty()) { requestLevel(nextLevel); return; }
    won = true;
    for (auto& o : objects)
        if (o->tag == "enemy" || o->tag == "projectile") o->alive = false;
    std::remove(SAVE_FILE);
}

std::string Engine::loadProgress() {
    std::ifstream file(SAVE_FILE);
    if (file) {
        try { return json::parse(file).value("level", "assets/level1.json"); } catch (...) {}
    }
    return "assets/level1.json";
}
