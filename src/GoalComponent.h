#pragma once
#include <string>
#include "SpriteComponent.h"


class GoalComponent : public Component {
public:
    GoalComponent(GameObject& parent, const std::string& unlockedTexture)
        : Component(parent), unlockedTexture(unlockedTexture) {}

    void update() override {
        if (Engine::won || Engine::collected < Engine::required) return;
        if (!unlocked) {
            unlocked = true;
            parent().get<SpriteComponent>()->setTexture(unlockedTexture);
        }
        auto player = findBody("player");
        if (player && player->overlaps(*parent().get<BodyComponent>())) Engine::finishLevel();
    }

    void draw() override {}

private:
    std::string unlockedTexture;
    bool unlocked = false;
};
