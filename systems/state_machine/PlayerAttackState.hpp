#pragma once

#include "IState.hpp"

namespace ks {

class PlayerAttackState : public IState {
public:
    void enter(Entity& owner) override;
    void update(Entity& owner, float dt) override;
    void exit(Entity& owner) override;
};

} // namespace ks
