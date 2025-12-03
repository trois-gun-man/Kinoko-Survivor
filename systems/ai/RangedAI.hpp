#pragma once

#include "AIStrategy.hpp"

namespace ks {

class RangedAI : public AIStrategy {
public:
    void decideAction(Enemy& owner, float dt) override;
};

} // namespace ks
