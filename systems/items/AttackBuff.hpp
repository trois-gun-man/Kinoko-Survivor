#pragma once

#include "BuffStrategy.hpp"

namespace ks {

class AttackBuff : public BuffStrategy {
public:
    void apply(Entity& target) override;
};

} // namespace ks
