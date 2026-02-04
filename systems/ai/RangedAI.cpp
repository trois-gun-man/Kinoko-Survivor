#include "RangedAI.hpp"

#include <cmath>

#include "../../entities/Enemy.hpp"

namespace ks {

void RangedAI::decideAction(Enemy& owner, float dt) {
    if (!owner.hasTarget()) {
        return;
    }

    const float preferred = 180.0f;
    const float tolerance = 20.0f;
    const float dx = owner.getTargetPosition().x - owner.getPosition().x;
    const float absDx = std::fabs(dx);

    float direction = 0.0f;
    if (absDx < preferred - tolerance) {
        direction = (dx > 0.0f) ? -1.0f : 1.0f; // back away
    } else if (absDx > preferred + tolerance) {
        direction = (dx > 0.0f) ? 1.0f : -1.0f; // advance until in range
    }

    owner.translate(direction * owner.getSpeed() * 0.65f * dt, 0.0f);
}

} // namespace ks
