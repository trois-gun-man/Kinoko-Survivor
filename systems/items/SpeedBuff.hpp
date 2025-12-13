#pragma once

#include "BuffStrategy.hpp"

namespace ks {

class SpeedBuff : public BuffStrategy {
public:
    void apply(Entity& target) override;
};

} // namespace ks
