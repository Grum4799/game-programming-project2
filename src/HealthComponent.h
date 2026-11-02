#pragma once
#include "SpriteComponent.h"

class HealthComponent : public Component {
public:
    float spawnX = 0, spawnY = 0;

    HealthComponent(GameObject& parent, int lives, float invulnerableTime)
        : Component(parent), lives(lives), invulnerableTime(invulnerableTime) {}

    void initialize() override {
        auto body = parent().get<BodyComponent>();
        spawnX = body->x;
        spawnY = body->y;
    }

    void hurt() {
        if (invulnerable > 0) return;
        if (--lives <= 0) { Engine::restartLevel(); return; }
        invulnerable = invulnerableTime;
        respawn();
    }

    void update() override {
        Engine::lives = lives;
        invulnerable -= Engine::dt;
        if (auto sprite = parent().get<SpriteComponent>())
            sprite->visible = invulnerable <= 0 || int(invulnerable * 10) % 2 == 0;

        if (parent().get<BodyComponent>()->y > Engine::height) {
            if (invulnerable > 0) respawn(); else hurt();
        }
    }

    void draw() override {}

private:
    void respawn() {
        auto body = parent().get<BodyComponent>();
        body->x = spawnX; body->y = spawnY;
        body->vx = body->vy = 0;
    }

    int lives;
    float invulnerableTime, invulnerable = 0;
};
