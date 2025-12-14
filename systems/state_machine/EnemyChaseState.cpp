#include "EnemyChaseState.hpp"

#include <algorithm>
#include <cmath>

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

// AI 戦略による移動と攻撃タイマーを更新する
void EnemyChaseState::update(Entity& owner, float dt) {
    auto* enemy = asEnemy(owner);
    if (!enemy) {
        return;
    }

    const float prevX = enemy->m_position.x();
    enemy->m_animTimer += dt;
    enemy->m_attackTimer = std::max(0.0f, enemy->m_attackTimer - dt);
    enemy->m_attackTriggered = false;
    enemy->m_isAttacking = false;

    if (enemy->m_strategy) {
        enemy->m_strategy->decideAction(*enemy, dt);
    }

    enemy->clampToBounds();
    enemy->updateAnimationState(prevX);

    if (enemy->hasTarget()) {
        const float dx = std::fabs(enemy->getTargetPosition().x - enemy->m_position.x());
        if (dx <= enemy->m_attackRange && enemy->m_attackTimer <= 0.0f) {
            enemy->m_attackTriggered = true;
            enemy->m_isAttacking = true;
            enemy->m_attackTimer = enemy->m_attackCooldown;
        }
    }
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
