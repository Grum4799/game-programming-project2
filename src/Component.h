#pragma once

class GameObject;


class Component {
public:
    Component(GameObject& parent) : _parent(parent) {}
    virtual ~Component() = default;

    virtual void initialize() {}
    virtual void update() = 0;
    virtual void draw() = 0;

    GameObject& parent() const { return _parent; }

private:
    GameObject& _parent;
};
