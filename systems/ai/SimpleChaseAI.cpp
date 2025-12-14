#include "SimpleChaseAI.hpp"

#include <cmath>

#include "../../entities/Enemy.hpp"

namespace ks {

void SimpleChaseAI::decideAction(Enemy& owner, float dt) {
    if (!owner.hasTarget()) {
        return;
    }

    const float dx = owner.getTargetPosition().x - owner.getPosition().x;
    if (std::fabs(dx) < 1.0f) {
        return;
    }

    const float direction = (dx > 0.0f) ? 1.0f : -1.0f;
    owner.translate(direction * owner.getSpeed() * dt, 0.0f);
}

} // namespace ks
