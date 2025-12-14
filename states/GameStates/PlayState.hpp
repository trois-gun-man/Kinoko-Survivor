#pragma once

#include <memory>
#include <vector>

#include <raylib.h>

#include "GameState.hpp"
#include "../../entities/Enemy.hpp"
#include "../../entities/Player.hpp"
#include "../../systems/factories/EnemySpawner.hpp"

namespace ks {

class PlayState : public GameState {
public:
    PlayState();
    ~PlayState() override;

    void Update(StateManager& manager) override;
    void Draw() override;

private:
    static constexpr const char* enemySpritePath = "assets/enemy_sprite.png";
    static constexpr const char* backgroundPath = "assets/play_background.png";
    static constexpr float kDefaultGroundY = 360.0f;
    static constexpr float kDefaultLaneLeft = 80.0f;
    static constexpr float kDefaultLaneRight = 720.0f;
    static constexpr int kDefaultEnemyDamage = 8;

    Player m_player;
    std::vector<Enemy> m_enemies;
    Texture2D m_background{};
    EnemySpawner m_spawner;
    float m_groundY = kDefaultGroundY;
    float m_laneLeft = kDefaultLaneLeft;
    float m_laneRight = kDefaultLaneRight;
    int m_enemyDamage = kDefaultEnemyDamage;
    float m_survivalTime = 0.0f;
    bool m_playerDefeated = false;

    void loadBackground();
    void updateEnemies(float dt);
    void drawBackground() const;
    void drawGround() const;
    void drawUI() const;
};

} // namespace ks
