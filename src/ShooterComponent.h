#pragma once
#include <cmath>
#include <string>
#include "BodyComponent.h"

class ShooterComponent : public Component {
public:
    ShooterComponent(GameObject& parent, const std::string& projectile, float cooldown, float range, float speed)
        : Component(parent), projectile(projectile), cooldown(cooldown), range(range), speed(speed), timer(cooldown) {}

    void update() override {
        timer -= Engine::dt;
        auto body = parent().get<BodyComponent>();
        auto player = findBody("player");
        if (!player || timer > 0) return;

        float dx = player->centerX() - body->centerX();
        float dy = player->centerY() - body->centerY();
        float dist = std::hypot(dx, dy);
        if (dist > range || dist < 1) return;

        Engine::spawn(projectile, {{"Body", {{"x", body->centerX() - 7}, {"y", body->centerY() - 7},
                                             {"vx", dx / dist * speed}, {"vy", dy / dist * speed}}}});
        timer = cooldown;
    }

    void draw() override {}

private:
    std::string projectile;
    float cooldown, range, speed, timer;
};
