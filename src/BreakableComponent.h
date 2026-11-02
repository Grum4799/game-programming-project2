#pragma once
#include <cstdlib>
#include "SpriteComponent.h"

class BreakableComponent : public Component {
public:
    BreakableComponent(GameObject& parent, float breakTime, float respawnTime)
        : Component(parent), breakTime(breakTime), respawnTime(respawnTime) {}

    void update() override {
        auto body = parent().get<BodyComponent>();
        auto sprite = parent().get<SpriteComponent>();
        auto player = findBody("player");

        if (broken) {
            timer += Engine::dt;
            if (timer >= respawnTime && !(player && player->overlaps(*body))) { 
                broken = false;
                timer = 0;
                body->solid = sprite->visible = true;
            }
            return;
        }

        bool stoodOn = player && player->ground == &parent();
        timer = stoodOn ? timer + Engine::dt : std::max(0.0f, timer - Engine::dt);

        sprite->alpha = Uint8(255 - 175 * (timer / breakTime));
        sprite->offsetX = timer > 0 ? std::rand() % 5 - 2 : 0;

        if (timer >= breakTime) {
            broken = true;
            timer = 0;
            body->solid = sprite->visible = false;
        }
    }

    void draw() override {}

private:
    float breakTime, respawnTime, timer = 0;
    bool broken = false;
};
