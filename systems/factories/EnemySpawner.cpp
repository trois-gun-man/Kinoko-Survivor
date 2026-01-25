#include "EnemySpawner.hpp"

#include <utility>

#include <raylib.h>

#include "../../entities/Enemy.hpp"

namespace ks {

namespace {
constexpr float kSpawnOffset = 60.0f;
constexpr float kLaneBuffer = 30.0f;
constexpr int kEnemySpawnFlgBit = 0x1; // 0x1: エネミーを出す、0x0: 無効化
} // namespace

void EnemySpawner::setLane(float groundY, float minX, float maxX) {
    m_groundY = groundY;
    m_laneLeft = minX;
    m_laneRight = maxX;
}

void EnemySpawner::update(float dt, std::vector<Enemy>& outEnemies) {
    if (m_spawned) {
        return;
    }

    m_timer += dt;
    if (m_timer < m_interval) {
        return;
    }

    m_timer = 0.0f;
    spawn(outEnemies);
    m_spawned = false;
}

void EnemySpawner::spawn(std::vector<Enemy>& outEnemies) {
    if (kEnemySpawnFlgBit == 0x0) {
        return;
    }
    Enemy enemy;
    enemy.setGround(m_groundY);
    enemy.setMovementBounds(m_laneLeft - kLaneBuffer, m_laneRight);

    if (GetRandomValue(0, 1) == 0) {
        enemy.setPosition(m_laneLeft - kSpawnOffset, m_groundY);
    } else {
        enemy.setPosition(m_laneRight + kSpawnOffset, m_groundY);
    }
    outEnemies.push_back(std::move(enemy));
}

} // namespace ks
