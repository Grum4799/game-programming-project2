#pragma once
#include <cmath>
#include "BodyComponent.h"

class PatrolChaseComponent : public Component {
public:
    enum class State { Patrol, Chase };
    State state = State::Patrol;

    PatrolChaseComponent(GameObject& parent, float left, float right, float speed, float chaseSpeed, float range)
        : Component(parent), left(left), right(right), speed(speed), chaseSpeed(chaseSpeed), range(range) {}

    void update() override {
        auto body = parent().get<BodyComponent>();
        auto player = findBody("player");

        state = State::Patrol;
        if (player && std::hypot(player->centerX() - body->centerX(), player->centerY() - body->centerY()) < range)
            state = State::Chase;

        if (state == State::Chase) {
            float dx = player->centerX() - body->centerX();
            body->vx = std::abs(dx) < 4 ? 0 : (dx > 0 ? chaseSpeed : -chaseSpeed);
        } else {
            if (body->x <= left)  dir = 1;
            if (body->x >= right) dir = -1;
            body->vx = dir * speed;
        }
        body->x = std::clamp(body->x, left, right);
    }

    void draw() override {}

private:
    float left, right, speed, chaseSpeed, range;
    int dir = 1;
};
