#pragma once

namespace ks {

class Entity {
public:
    virtual ~Entity() = default;
    virtual void update(float dt) = 0; // TODO: inject behavior components
    virtual void render() = 0;
};

} // namespace ks
