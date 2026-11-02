#pragma once
#include "BodyComponent.h"
#include "Input.h"

class ControllerComponent : public Component {
public:
    ControllerComponent(GameObject& parent, float speed, float jumpSpeed)
        : Component(parent), speed(speed), jumpSpeed(jumpSpeed) {}

    void update() override {
        auto body = parent().get<BodyComponent>();
        int dir = 0;
        if (Input::isKeyDown(SDLK_a) || Input::isKeyDown(SDLK_LEFT))  dir -= 1;
        if (Input::isKeyDown(SDLK_d) || Input::isKeyDown(SDLK_RIGHT)) dir += 1;
        body->vx = dir * speed;

        bool jump = Input::isKeyDown(SDLK_SPACE) || Input::isKeyDown(SDLK_w) || Input::isKeyDown(SDLK_UP);
        if (jump && body->ground) body->vy = -jumpSpeed;
    }

    void draw() override {}

private:
    float speed, jumpSpeed;
};
