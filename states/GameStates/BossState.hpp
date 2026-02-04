#pragma once

#include "GameState.hpp"
#include "../../entities/Player.hpp"
#include "../../entities/Boss.hpp"
#include <raylib.h>
#include <vector>
#include <memory>

namespace ks {

namespace {
constexpr Color kGroundColor{255, 0, 0, 255};
constexpr int kGroundThickness = 12;
constexpr int kGroundOffset = 6;
} // namespace

class BossState : public GameState {
public:
    explicit BossState(std::unique_ptr<Player> player);
    ~BossState() override;
    void Update(StateManager& manager) override;
    void Draw() override;
	void drawGround() const;
    void drawPlayerHp() const;
	void stopBgm();
private:
	std::unique_ptr<Player> m_player;
	Boss m_boss;
	float m_groundY = 400.0f;
    float m_stageTimer = 0.0f;
	float m_stageElapsed = 0.0f;
	bool m_resultTriggered = false;
	Texture2D m_background{};
	float m_contactTimer = 0.0f;
	struct Shockwave {
		Rectangle bounds{};
		float travelled = 0.0f;
		float baseY = 0.0f;
		float wobble = 0.0f;
		bool active = true;
	};

	void loadBackground();
	void drawBackground() const;
	void spawnShockwave();
	void updateShockwaves(float dt);
	void drawShockwaves() const;
	void drawBossHp() const;
	Music m_bgm{};

	std::vector<Shockwave> m_shockwaves;
};

} // namespace ks
