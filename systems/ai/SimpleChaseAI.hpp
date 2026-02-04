#pragma once

#include "AIStrategy.hpp"

namespace ks {

class Enemy;

class SimpleChaseAI : public AIStrategy {
public:
    void decideAction(Enemy& owner, float dt) override;
};

} // namespace ks
