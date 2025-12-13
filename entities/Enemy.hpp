#pragma once

#include "Entity.hpp"

namespace ks {

class Enemy : public Entity {
public:
    void update(float dt) override;
    void render() override;
};

} // namespace ks
