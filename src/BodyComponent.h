#pragma once
#include <algorithm>
#include "Engine.h"

class BodyComponent : public Component {
public:
    float x, y, w, h, vx, vy, angle;
    bool gravity, solid, collides;

    GameObject* ground = nullptr;  
    bool hitSolid = false;          

    BodyComponent(GameObject& parent, float x, float y, float w, float h, float vx, float vy,
                  float angle, bool gravity, bool solid, bool collides)
        : Component(parent), x(x), y(y), w(w), h(h), vx(vx), vy(vy), angle(angle),
          gravity(gravity), solid(solid), collides(collides) {}

    float centerX() const { return x + w / 2; }
    float centerY() const { return y + h / 2; }

    bool overlaps(const BodyComponent& o) const {
        return x < o.x + o.w && x + w > o.x && y < o.y + o.h && y + h > o.y;
    }

    void update() override {
        const float dt = Engine::dt;
        if (gravity) vy = std::min(vy + 1800.0f * dt, 900.0f);
        ground = nullptr;
        hitSolid = false;

        x += vx * dt;
        if (collides) forEachSolidHit([&](BodyComponent& s) {
            x = vx > 0 ? s.x - w : s.x + s.w;
        });
        y += vy * dt;
        if (collides) forEachSolidHit([&](BodyComponent& s) {
            if (vy > 0) { y = s.y - h; ground = &s.parent(); }
            else        { y = s.y + s.h; }
            vy = 0;
        });
    }

    void draw() override {}

private:
    template <typename F>
    void forEachSolidHit(F resolve) {
        for (auto& o : Engine::objects) {
            BodyComponent* s = o->get<BodyComponent>();
            if (o.get() != &parent() && s && s->solid && overlaps(*s)) {
                resolve(*s);
                hitSolid = true;
            }
        }
    }
};

inline BodyComponent* findBody(const std::string& tag) {
    GameObject* o = Engine::find(tag);
    return o ? o->get<BodyComponent>() : nullptr;
}
