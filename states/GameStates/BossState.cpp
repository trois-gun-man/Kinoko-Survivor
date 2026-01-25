#include "BossState.hpp"

namespace ks {

BossState::BossState() {
	// コンストラクタ内で必要な初期化を行う
	float adjusted_player_yPosition = m_groundY - 42.0f;
	m_player.setGround(adjusted_player_yPosition);
	float groundY = 225.0f;
	float bossX = GetScreenWidth() - 250.0f;

	m_boss.setGround(groundY);
	m_boss.setPosition(bossX, groundY);
}
void BossState::Update(StateManager&) {
	float dt = GetFrameTime();
	m_player.update(dt);
	m_boss.update(dt);

	if (CheckCollisionRecs(
		m_player.getAttackHitBox(),
		m_boss.getHitBox()
	    ) && m_player.isAttacking() && !m_player.getAttackHit()) {
			m_boss.applyDamage(10);
			m_player.setAttackHit(true);
		}
	DrawRectangleLinesEx(
		m_boss.getHitBox(), 2.0f, BLUE
	);

}

void BossState::Draw() {
//    ClearBackground(Color{0, 0, 0, 255}); // 暗い赤背景
//
//    DrawText(
//        "BOSS STAGE",
//        GetScreenWidth() / 2 - 120,
//        GetScreenHeight() / 2 - 20,
//        40,
//        RED
//    );

ClearBackground(BLACK);
drawGround();

m_player.render();
m_boss.render();
m_player.draw();

}

void BossState::drawGround() const {
    //std::cout << "Draw Ground!" << std::endl;
    const Rectangle soil{0.0f, m_groundY, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) - m_groundY};
 //   DrawRectangleRec(soil, kSoilColor);
    DrawRectangle(0, static_cast<int>(m_groundY) - kGroundOffset, GetScreenWidth(), kGroundThickness, kGroundColor);
}


} // namespace ks
