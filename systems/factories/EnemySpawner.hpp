#pragma once

#include <vector>

namespace ks {

class Enemy;
class Player;

class EnemySpawner {
public:
    void setPlayer(Player* player);
    void setLane(float groundY, float minX, float maxX);
    void configureWave(int index);
    void update(float dt, std::vector<Enemy>& outEnemies);
    void spawnWave(std::vector<Enemy>& outEnemies);

private:
    void spawnOne(std::vector<Enemy>& outEnemies);

    Player* m_player = nullptr;
    float m_groundY = 360.0f;
    float m_laneLeft = 0.0f;
    float m_laneRight = 800.0f;
    float m_timer = 0.0f;
    float m_interval = 3.0f;
    int m_waveIndex = 0;
    int m_waveCount = 3;
    int m_spawnedThisWave = 0;
};

} // namespace ks
