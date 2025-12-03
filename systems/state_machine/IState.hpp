#pragma once

namespace ks {

class Entity;

class IState {
public:
    virtual ~IState() = default;
    virtual void enter(Entity& owner) = 0;
    virtual void update(Entity& owner, float dt) = 0;
    virtual void exit(Entity& owner) = 0;
};

} // namespace ks
