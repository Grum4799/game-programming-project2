#pragma once
#include <functional>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include "GameObject.h"

using json = nlohmann::json;

struct ComponentFactory {
    static std::map<std::string, std::function<void(GameObject&, const json&)>> builders;
    static void add(GameObject& obj, const json& data);
};
