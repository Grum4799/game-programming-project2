#pragma once
#include "BodyComponent.h"

class ProjectileComponent : public Component {
public:
    ProjectileComponent(GameObject& parent, float lifetime) : Component(parent), lifetime(lifetime) {}

    void update() override {
        auto body = parent().get<BodyComponent>();
        body->angle += 540 * Engine::dt;
        lifetime -= Engine::dt;
        if (lifetime <= 0 || body->hitSolid) parent().alive = false;
    }

    void draw() override {}

private:
    float lifetime;
};
