#include "EnemySpawner.hpp"

#include <algorithm>
#include <memory>

#include <raylib.h>

#include "../../entities/Enemy.hpp"
#include "../../entities/Player.hpp"
#include "../../systems/ai/RangedAI.hpp"
#include "../../systems/ai/SimpleChaseAI.hpp"

namespace ks {

namespace {
constexpr int kBaseWaveCount = 3;
constexpr float kBaseInterval = 3.0f;
constexpr float kIntervalStep = 0.2f;
constexpr float kMinimumInterval = 0.8f;
constexpr float kSpawnOffset = 60.0f;
constexpr float kLaneBuffer = 200.0f;
constexpr int kRangedSpawnThreshold = 70;
} // namespace

void EnemySpawner::setPlayer(Player* player) {
    m_player = player;
}

void EnemySpawner::setLane(float groundY, float minX, float maxX) {
    m_groundY = groundY;
    m_laneLeft = minX;
    m_laneRight = maxX;
}

void EnemySpawner::configureWave(int index) {
    m_waveIndex = std::max(0, index);
    m_waveCount = kBaseWaveCount + m_waveIndex;
    m_interval = std::max(kMinimumInterval, kBaseInterval - static_cast<float>(m_waveIndex) * kIntervalStep);
    m_spawnedThisWave = 0;
    m_timer = 0.0f;
}

void EnemySpawner::update(float dt, std::vector<Enemy>& outEnemies) {
    m_timer += dt;
    if (m_timer < m_interval) {
        return;
    }

    m_timer = 0.0f;
    spawnOne(outEnemies);
    ++m_spawnedThisWave;

    if (m_spawnedThisWave >= m_waveCount) {
        configureWave(m_waveIndex + 1);
    }
}

void EnemySpawner::spawnWave(std::vector<Enemy>& outEnemies) {
    for (int i = 0; i < m_waveCount; ++i) {
        spawnOne(outEnemies);
    }
    m_spawnedThisWave = 0;
    m_timer = 0.0f;
}

void EnemySpawner::spawnOne(std::vector<Enemy>& outEnemies) {
    Enemy enemy;
    enemy.setGround(m_groundY);
    enemy.setMovementBounds(m_laneLeft - kLaneBuffer, m_laneRight + kLaneBuffer);

    if (GetRandomValue(0, 1) == 0) {
        enemy.setPosition(m_laneLeft - kSpawnOffset, m_groundY);
    } else {
        enemy.setPosition(m_laneRight + kSpawnOffset, m_groundY);
    }

    if (GetRandomValue(0, 100) > kRangedSpawnThreshold) {
        enemy.setStrategy(std::make_unique<RangedAI>());
    } else {
        enemy.setStrategy(std::make_unique<SimpleChaseAI>());
    }

    enemy.setTarget(m_player);
    outEnemies.push_back(std::move(enemy));
}

} // namespace ks
