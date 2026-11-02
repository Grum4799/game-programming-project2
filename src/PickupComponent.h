#pragma once
#include <cmath>
#include "BodyComponent.h"

class PickupComponent : public Component {
public:
    PickupComponent(GameObject& parent, int value) : Component(parent), value(value) {}

    void initialize() override { baseY = parent().get<BodyComponent>()->y; }

    void update() override {
        auto body = parent().get<BodyComponent>();
        t += Engine::dt;
        body->y = baseY + std::sin(t * 4) * 4;

        auto player = findBody("player");
        if (player && player->overlaps(*body)) {
            Engine::collected += value;
            parent().alive = false;
        }
    }

    void draw() override {}

private:
    int value;
    float baseY = 0, t = 0;
};
