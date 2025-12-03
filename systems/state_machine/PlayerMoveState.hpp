#pragma once

#include "IState.hpp"

namespace ks {

class Player;

class PlayerMoveState : public IState {
public:
    void enter(Entity& owner) override;
    void update(Entity& owner, float dt) override;
    void exit(Entity& owner) override;
};

} // namespace ks
