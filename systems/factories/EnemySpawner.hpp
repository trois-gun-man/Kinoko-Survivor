#pragma once

#include <vector>

namespace ks {

class Enemy;
class EnemySpawner {
public:
    void setLane(float groundY, float minX, float maxX);
    void update(float dt, std::vector<Enemy>& outEnemies);

private:
    void spawn(std::vector<Enemy>& outEnemies);

    float m_groundY = 360.0f;
    float m_laneLeft = 0.0f;
    float m_laneRight = 800.0f;
    float m_timer = 0.0f;
    float m_interval = 5.0f;
    bool m_spawned = false;
};

} // namespace ks
