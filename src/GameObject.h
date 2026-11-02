#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Component.h"

class GameObject {
public:
    std::string tag;
    bool alive = true;

    template <typename T, typename... Args>
    void add(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        components.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
    }

    template <typename T>
    T* get() {
        for (auto& c : components)
            if (T* found = dynamic_cast<T*>(c.get())) return found;
        return nullptr;
    }

    void initialize() { for (auto& c : components) c->initialize(); }
    void update()     { for (auto& c : components) c->update(); }
    void draw()       { for (auto& c : components) c->draw(); }

private:
    std::vector<std::unique_ptr<Component>> components;
};
