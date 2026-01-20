#pragma once

#include "GameState.hpp"
#include "../../entities/Player.hpp"
#include "../../entities/Boss.hpp"
#include <raylib.h>

namespace ks {

namespace {
constexpr Color kGroundColor{255, 0, 0, 255};
constexpr int kGroundThickness = 12;
constexpr int kGroundOffset = 6;
} // namespace

class BossState : public GameState {
public:
    BossState();

    void Update(StateManager& manager) override;
    void Draw() override;
	void drawGround() const;

private:
	Player m_player;
	Boss m_boss;
	float m_groundY = 400.0f;
};

} // namespace ks
