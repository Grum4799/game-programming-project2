#pragma once
#include "HealthComponent.h"

class HazardComponent : public Component {
public:
    HazardComponent(GameObject& parent, bool destroyOnHit) : Component(parent), destroyOnHit(destroyOnHit) {}

    void update() override {
        GameObject* player = Engine::find("player");
        if (!player || !player->get<BodyComponent>()->overlaps(*parent().get<BodyComponent>())) return;
        player->get<HealthComponent>()->hurt();
        if (destroyOnHit) parent().alive = false;
    }

    void draw() override {}

private:
    bool destroyOnHit;
};
