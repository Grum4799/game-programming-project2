#define SDL_MAIN_HANDLED
#include "Engine.h"
#include "Input.h"

std::set<SDL_Keycode> Input::keysDown;

int main(int argc, char* argv[]) {
    if (!Engine::init("Power Runner", 960, 540)) return 1;
    Engine::loadPrefabs("assets/prefabs.json");
    Engine::requestLevel(Engine::loadProgress());
    Engine::run();
    return 0;
}
