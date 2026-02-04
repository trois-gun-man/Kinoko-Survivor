#include "PlayState.hpp"

#include <algorithm>
#include <iostream>

#include "ResultState.hpp"
#include "BossState.hpp"

namespace ks {

namespace {
constexpr float kLaneCenterFactor = 0.5f;
constexpr Color kClearColor{15, 12, 35, 255};
constexpr Color kSoilColor{78, 52, 46, 255};
constexpr Color kGrassColor{105, 190, 92, 255};
constexpr int kGrassThickness = 12;
constexpr int kGrassOffset = 6;
constexpr int kUiPadding = 20;
constexpr int kMoveTextY = 24;
constexpr int kMoveTextSize = 20;
constexpr int kHintTextY = 48;
constexpr int kHintTextSize = 18;
constexpr int kTimerTextY = 72;
constexpr int kTimerTextSize = 24;
constexpr Color kMoveTextColor{220, 220, 235, 255};
constexpr Color kHintTextColor{180, 180, 210, 255};
constexpr Color kTimerTextColor{200, 220, 255, 255};
constexpr Color kHpBackgroundColor{40, 40, 70, 200};
constexpr Color kHpFillColor{120, 220, 160, 255};
constexpr Color kHpOutlineColor{230, 240, 255, 80};
constexpr Color kHpLabelColor{220, 220, 235, 255};
constexpr float kHpBarWidth = 220.0f;
constexpr float kHpBarHeight = 14.0f;
constexpr float kHpLabelOffsetX = 34.0f;
constexpr float kHpLabelOffsetY = 2.0f;
constexpr int kHpLabelFontSize = 20;
constexpr int kSecondsPerMinute = 60;
constexpr float kTimeRoundingBias = 0.5f;
constexpr char kMoveInstruction[] = "Move: A / D or <- ->";
constexpr char kAttackHint[] = "Attack dodging: stay out of red caps!";
constexpr char kTimeFormat[] = "Time %02d:%02d";
constexpr char kHpLabel[] = "HP";
} // namespace

PlayState::PlayState(): m_player(std::make_unique<Player>()) {
    loadBackground();
    Enemy::AcquireSpriteSheet(enemySpritePath); 
    float adjusted_player_yPosition = m_groundY - 42.0f;
    m_player->setGround(adjusted_player_yPosition); 
    m_player->setMovementBounds(m_laneLeft, m_laneRight);
    m_player->setPosition((m_laneLeft + m_laneRight) * kLaneCenterFactor);

    m_spawner.setPlayer(m_player.get());
    m_spawner.setLane(adjusted_player_yPosition, m_laneLeft, m_laneRight);
    m_spawner.configureWave(0);
    m_spawner.spawnWave(m_enemies);
}

PlayState::~PlayState() {
    if (m_background.id != 0) {
        UnloadTexture(m_background);
    }
    Enemy::ReleaseSpriteSheet();
}

void PlayState::Update(StateManager& manager) {
    const float dt = GetFrameTime();
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
        return;
    }

    if (!m_player->isDead()) {
        m_survivalTime += dt;
    }

    m_player->update(dt);
    updateEnemies(dt);
    m_spawner.update(dt, m_enemies);

    if (m_killedEnemies >= 5 && !m_playerDefeated) {
        manager.ChangeState<BossState>(std::move(m_player));
        return; // プレイヤーを移譲したら以降の処理で参照しない
    }

    if (m_player && m_player->isDead() && !m_playerDefeated) {
        m_playerDefeated = true;
        manager.ChangeState<ResultState>(m_survivalTime, [](StateManager& restartManager) {
            restartManager.ChangeState<PlayState>();
        });
    }
}

void PlayState::Draw() {
    drawBackground();
    drawGround();

    m_player->render();
   // std::cout << "Draw Player!" << std::endl;
    for (Enemy& enemy : m_enemies) {
        enemy.render();
    }
    m_player->draw();
    drawUI();
}

void PlayState::loadBackground() {
    if (FileExists(backgroundPath)) {
        m_background = LoadTexture(backgroundPath);
    }
}

void PlayState::updateEnemies(float dt) {
    for (Enemy& enemy : m_enemies) {
        enemy.setTarget(m_player.get());
        enemy.update(dt);

        // 敵がプレイヤーを攻撃していたらダメージを与える
        if (enemy.consumeAttackEvent()) {
            m_player->applyDamage(m_enemyDamage);
        }

        // プレイヤーが敵攻撃していたらダメージを与える
        if (m_player->isAttacking() && !enemy.isDead() && !m_player->getAttackHit()) {
            if (CheckCollisionRecs(m_player->getAttackHitBox(), enemy.getHitBox())) {
                enemy.applyDamage(m_player->getAttackPower()); // プレイヤーの攻撃力を10に設定
                m_player->setAttackHit(true);
            }
        }

        DrawRectangleLinesEx(
            m_player->getAttackHitBox(), 2.0f, BLUE
        );
        DrawRectangleLinesEx(
            enemy.getHitBox(), 2.0f, BLUE
        );
    }
    
    int before = m_enemies.size();

    m_enemies.erase(
        std::remove_if(
            m_enemies.begin(),
            m_enemies.end(),
            [](const Enemy& e) { return e.isDead(); }
        ),
        m_enemies.end()
    );

    int after = m_enemies.size();
    m_killedEnemies += (before - after);

}


void PlayState::drawBackground() const {
    //std::cout << "Clear Background!" << std::endl;
    ClearBackground(kClearColor);

    if (m_background.id == 0) {
        return;
    }

    const float screenW = static_cast<float>(GetScreenWidth());
    const float screenH = static_cast<float>(GetScreenHeight());
    const float scaleX = screenW / static_cast<float>(m_background.width);
    const float scaleY = screenH / static_cast<float>(m_background.height);
    const float scale = std::max(scaleX, scaleY);
    const Rectangle src{0.0f, 0.0f, static_cast<float>(m_background.width), static_cast<float>(m_background.height)};
    const Rectangle dest{0.0f, 0.0f, m_background.width * scale, m_background.height * scale};
    //std::cout << "Draw Background!" << std::endl;
    DrawTexturePro(m_background, src, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

void PlayState::drawGround() const {
    //std::cout << "Draw Ground!" << std::endl;
    const Rectangle soil{0.0f, m_groundY, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) - m_groundY};
    DrawRectangleRec(soil, kSoilColor);
    DrawRectangle(0, static_cast<int>(m_groundY) - kGrassOffset, GetScreenWidth(), kGrassThickness, kGrassColor);
}

void PlayState::drawUI() const {
    //std::cout << "Draw UI!" << std::endl;
    const int padding = kUiPadding;
    DrawText(kMoveInstruction, padding, kMoveTextY, kMoveTextSize, kMoveTextColor);
    DrawText(kAttackHint, padding, kHintTextY, kHintTextSize, kHintTextColor);

    const int totalSeconds = static_cast<int>(m_survivalTime + kTimeRoundingBias);
    const int minutes = totalSeconds / kSecondsPerMinute;
    const int seconds = totalSeconds % kSecondsPerMinute;
    const char* timeText = TextFormat(kTimeFormat, minutes, seconds);
    DrawText(timeText, padding, kTimerTextY, kTimerTextSize, kTimerTextColor);

    const float barWidth = kHpBarWidth;
    const float barHeight = kHpBarHeight;
    const float x = static_cast<float>(GetScreenWidth()) - barWidth - padding;
    const float y = padding;
    const int maxHp = std::max(1, m_player->maxHealth());
    const float ratio = static_cast<float>(m_player->health()) / static_cast<float>(maxHp);

    DrawRectangleRec(Rectangle{x, y, barWidth, barHeight}, kHpBackgroundColor);
    DrawRectangleRec(Rectangle{x, y, barWidth * ratio, barHeight}, kHpFillColor);
    DrawRectangleLines(static_cast<int>(x), static_cast<int>(y), static_cast<int>(barWidth), static_cast<int>(barHeight), kHpOutlineColor);

    DrawText(kHpLabel, static_cast<int>(x - kHpLabelOffsetX), static_cast<int>(y - kHpLabelOffsetY), kHpLabelFontSize, kHpLabelColor);
}

} // namespace ks
