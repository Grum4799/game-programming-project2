#pragma once
#include <string>
#include "HealthComponent.h"

class CheckpointComponent : public Component {
public:
    CheckpointComponent(GameObject& parent, const std::string& activeTexture)
        : Component(parent), activeTexture(activeTexture) {}

    void update() override {
        GameObject* player = Engine::find("player");
        auto body = parent().get<BodyComponent>();
        if (active || !player) return;
        auto pBody = player->get<BodyComponent>();
        if (!pBody->overlaps(*body)) return;

        active = true;
        auto health = player->get<HealthComponent>();
        health->spawnX = body->centerX() - pBody->w / 2;
        health->spawnY = body->y + body->h - pBody->h;
        parent().get<SpriteComponent>()->setTexture(activeTexture);
    }

    void draw() override {}

private:
    std::string activeTexture;
    bool active = false;
};
