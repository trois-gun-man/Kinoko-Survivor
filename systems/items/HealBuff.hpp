#pragma once

#include "BuffStrategy.hpp"

namespace ks {

class HealBuff : public BuffStrategy {
public:
    void apply(Entity& target) override;
};

} // namespace ks
