#include "EnemyChaseState.hpp"

#include "../../entities/Enemy.hpp"

namespace ks {

namespace {
Enemy* asEnemy(Entity& owner) {
    return dynamic_cast<Enemy*>(&owner);
}
} // namespace

// 状態開始時にアニメーションタイマーをリセットする
void EnemyChaseState::enter(Entity& owner) {
    auto* enemy = asEnemy(owner);
    if (!enemy) {
        return;
    }
    enemy->m_animTimer = 0.0f;
}

// シンプルな左右移動のみを行う
void EnemyChaseState::update(Entity& owner, float dt) {
    auto* enemy = asEnemy(owner);
    if (!enemy) {
        return;
    }

    const float prevX = enemy->m_position.x();
    enemy->m_animTimer += dt;
    enemy->m_attackTriggered = false;
    enemy->m_isAttacking = false;

    const float velocity = enemy->m_patrolDirection * enemy->m_speed;
    enemy->translate(velocity * dt, 0.0f);

    const float leftLimit = enemy->m_minX + enemy->m_radius.x;
    const float rightLimit = enemy->m_maxX - enemy->m_radius.x;
    const float currentX = enemy->m_position.x();

    if (currentX <= leftLimit) {
        enemy->m_position.set(leftLimit, enemy->m_groundY);
        enemy->m_patrolDirection = 1.0f;
    } else if (currentX >= rightLimit) {
        enemy->m_position.set(rightLimit, enemy->m_groundY);
        enemy->m_patrolDirection = -1.0f;
    }

    enemy->clampToBounds();
    enemy->updateAnimationState(prevX);
}

// 状態を抜ける際に移動フラグを下ろす
void EnemyChaseState::exit(Entity& owner) {
    auto* enemy = asEnemy(owner);
    if (!enemy) {
        return;
    }
    enemy->m_isMoving = false;
}

} // namespace ks
