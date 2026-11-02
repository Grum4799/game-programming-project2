#include "ComponentFactory.h"
#include <iostream>
#include "BodyComponent.h"
#include "BreakableComponent.h"
#include "CheckpointComponent.h"
#include "ControllerComponent.h"
#include "GoalComponent.h"
#include "HazardComponent.h"
#include "HealthComponent.h"
#include "PatrolChaseComponent.h"
#include "PickupComponent.h"
#include "ProjectileComponent.h"
#include "ShooterComponent.h"
#include "SpriteComponent.h"

std::map<std::string, std::function<void(GameObject&, const json&)>> ComponentFactory::builders = {
    {"Body", [](GameObject& o, const json& j) {
        o.add<BodyComponent>(j.value("x", 0.f), j.value("y", 0.f), j.value("w", 32.f), j.value("h", 32.f),
                             j.value("vx", 0.f), j.value("vy", 0.f), j.value("angle", 0.f),
                             j.value("gravity", false), j.value("solid", false), j.value("collides", false));
    }},
    {"Sprite", [](GameObject& o, const json& j) {
        o.add<SpriteComponent>(j.at("texture").get<std::string>(), j.value("flip", false), j.value("tile", false));
    }},
    {"Controller", [](GameObject& o, const json& j) {
        o.add<ControllerComponent>(j.value("speed", 260.f), j.value("jump", 700.f));
    }},
    {"Health", [](GameObject& o, const json& j) {
        o.add<HealthComponent>(j.value("lives", 3), j.value("invulnerable", 1.5f));
    }},
    {"PatrolChase", [](GameObject& o, const json& j) {
        o.add<PatrolChaseComponent>(j.at("left").get<float>(), j.at("right").get<float>(),
                                    j.value("speed", 80.f), j.value("chaseSpeed", 150.f), j.value("range", 250.f));
    }},
    {"Shooter", [](GameObject& o, const json& j) {
        o.add<ShooterComponent>(j.value("projectile", "EnergyBall"), j.value("cooldown", 1.5f),
                                j.value("range", 300.f), j.value("speed", 260.f));
    }},
    {"Projectile", [](GameObject& o, const json& j) { o.add<ProjectileComponent>(j.value("lifetime", 3.f)); }},
    {"Hazard",     [](GameObject& o, const json& j) { o.add<HazardComponent>(j.value("destroyOnHit", false)); }},
    {"Breakable",  [](GameObject& o, const json& j) {
        o.add<BreakableComponent>(j.value("breakTime", 1.f), j.value("respawnTime", 3.f));
    }},
    {"Pickup",     [](GameObject& o, const json& j) { o.add<PickupComponent>(j.value("value", 1)); }},
    {"Goal",       [](GameObject& o, const json& j) { o.add<GoalComponent>(j.at("unlockedTexture").get<std::string>()); }},
    {"Checkpoint", [](GameObject& o, const json& j) { o.add<CheckpointComponent>(j.at("activeTexture").get<std::string>()); }},
};

void ComponentFactory::add(GameObject& obj, const json& data) {
    std::string type = data.at("type");
    auto it = builders.find(type);
    if (it == builders.end()) { std::cerr << "[Factory] Unknown component: " << type << "\n"; return; }
    it->second(obj, data);
}
