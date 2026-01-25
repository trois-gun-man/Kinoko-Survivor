#include "BossState.hpp"
#include "ResultState.hpp"
#include "PlayState.hpp"

#include <algorithm>
#include <cmath>

namespace {
constexpr float kShockwaveSpeed = 360.0f;
constexpr float kShockwaveRange = 200.0f;
constexpr Vector2 kShockwaveSize{120.0f, 120.0f};
constexpr Color kShockwaveColor{255, 220, 120, 220};
constexpr Color kShockwaveEdgeColor{255, 255, 255, 160};
constexpr Color kShockwaveDustColor{255, 255, 255, 120};
constexpr float kShockwaveBobAmplitude = 6.0f;
constexpr float kShockwaveBobSpeed = 12.0f;
constexpr int kShockwaveDamage = 20;
constexpr float kBossHpBarWidth = 320.0f;
constexpr float kBossHpBarHeight = 20.0f;
constexpr int kBossHpFontSize = 22;
constexpr Color kBossHpTextColor{240, 240, 255, 255};
constexpr Color kBossHpBarBg{30, 30, 60, 200};
constexpr Color kBossHpBarFill{220, 60, 80, 255};
constexpr Color kBossHpBarOutline{255, 255, 255, 80};
constexpr float kPlayerHpBarWidth = 220.0f;
constexpr float kPlayerHpBarHeight = 16.0f;
constexpr int kPlayerHpFontSize = 20;
constexpr Color kPlayerHpLabelColor{220, 220, 235, 255};
constexpr Color kPlayerHpBarBg{30, 70, 40, 200};
constexpr Color kPlayerHpBarFill{90, 200, 120, 255};
constexpr Color kPlayerHpBarOutline{200, 255, 200, 80};
constexpr int kUiPadding = 20;
constexpr int kStageTextFontSize = 42;
constexpr Color kStageTextColor{255, 120, 120, 255};
constexpr Color kStageShadowColor{0, 0, 0, 160};
constexpr float kStageTextDuration = 3.5f;
constexpr float kStageTextFadeOut = 1.0f;
} // namespace

namespace ks {

BossState::BossState() {
	// コンストラクタ内で必要な初期化を行う
	float adjusted_player_yPosition = m_groundY - 42.0f;
	m_player.setGround(adjusted_player_yPosition);
	float groundY = 310.0f;
	float bossX = GetScreenWidth() - 160.0f;

	m_boss.setGround(groundY);
	m_boss.setPosition(bossX, groundY);
    m_stageTimer = kStageTextDuration;
}
void BossState::Update(StateManager& manager) {
	const float dt = GetFrameTime();
	if (!m_resultTriggered) {
		m_stageElapsed += dt;
	}

	m_player.update(dt);
	m_boss.update(dt);

	if (m_boss.consumeShockwaveEvent()) {
		spawnShockwave();
	}

	updateShockwaves(dt);

	if (!m_player.isDead() && !m_boss.isDead()) {
		if (CheckCollisionRecs(
			m_player.getAttackHitBox(),
			m_boss.getHitBox())) {
			m_boss.applyDamage(10);
		}
	}
	DrawRectangleLinesEx(
		m_boss.getHitBox(), 2.0f, BLUE
	);

	if (!m_resultTriggered && (m_player.isDead() || m_boss.isDead())) {
		m_resultTriggered = true;
		manager.ChangeState<ResultState>(m_stageElapsed, [](StateManager& restartManager) {
			restartManager.ChangeState<PlayState>();
		});
		return;
	}

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
drawShockwaves();
m_player.draw();
drawBossHp();
drawPlayerHp();
if (m_stageTimer > 0.0f) {
	m_stageTimer -= GetFrameTime();
}

if (m_stageTimer > -kStageTextFadeOut) {
	const float elapsed = kStageTextDuration - std::max(0.0f, m_stageTimer);
	float alpha = 1.0f;
	if (m_stageTimer < 0.0f) {
		alpha = 1.0f - std::min(1.0f, (-m_stageTimer) / kStageTextFadeOut);
	}
	const char* message = "BOSS STAGE";
	const float width = MeasureText(message, kStageTextFontSize);
	const float x = (GetScreenWidth() - width) * 0.5f;
	const float y = 64.0f;
	Color shadow = kStageShadowColor;
	shadow.a = static_cast<unsigned char>(static_cast<float>(shadow.a) * alpha);
	Color text = kStageTextColor;
	text.a = static_cast<unsigned char>(static_cast<float>(text.a) * alpha);
	DrawText(message, static_cast<int>(x + 3.0f), static_cast<int>(y + 3.0f), kStageTextFontSize, shadow);
	DrawText(message, static_cast<int>(x), static_cast<int>(y), kStageTextFontSize, text);
}

}
void BossState::spawnShockwave() {
	Shockwave wave{};
	const Rectangle bossHit = m_boss.getHitBox();
	const float baseY = m_groundY - kShockwaveSize.y + 4.0f;
	wave.bounds = Rectangle{
		bossHit.x - kShockwaveSize.x,
		baseY,
		kShockwaveSize.x,
		kShockwaveSize.y
	};
	wave.travelled = 0.0f;
	wave.baseY = wave.bounds.y;
	wave.wobble = 0.0f;
	wave.active = true;
	m_shockwaves.push_back(wave);
}

void BossState::updateShockwaves(float dt) {
	if (m_shockwaves.empty()) {
		return;
	}

	const float delta = kShockwaveSpeed * dt;
	const Vector2 playerPos = m_player.getPosition();
	const Vector2 playerRadius = m_player.radius();
	Rectangle playerBounds{playerPos.x, playerPos.y, playerRadius.x, playerRadius.y};

	for (Shockwave& wave : m_shockwaves) {
		if (!wave.active) {
			continue;
		}

		wave.bounds.x -= delta;
		wave.travelled += delta;
		wave.wobble += dt * kShockwaveBobSpeed;
		wave.bounds.y = wave.baseY + std::sin(wave.wobble) * kShockwaveBobAmplitude;
		if (wave.travelled >= kShockwaveRange) {
			wave.active = false;
			continue;
		}

		if (!m_player.isDead() && CheckCollisionRecs(wave.bounds, playerBounds)) {
			m_player.applyDamage(kShockwaveDamage);
			wave.active = false;
		}
	}

	m_shockwaves.erase(
		std::remove_if(
			m_shockwaves.begin(),
			m_shockwaves.end(),
			[](const Shockwave& wave) { return !wave.active; }
		),
		m_shockwaves.end()
	);
}

void BossState::drawShockwaves() const {
	for (const Shockwave& wave : m_shockwaves) {
		if (!wave.active) {
			continue;
		}
		const float progress = wave.travelled / kShockwaveRange;
		const float alphaFactor = 1.0f - (progress * 0.5f);
		Color bodyColor = kShockwaveColor;
		bodyColor.a = static_cast<unsigned char>(static_cast<float>(bodyColor.a) * std::clamp(alphaFactor, 0.2f, 1.0f));
		DrawRectangleRec(wave.bounds, bodyColor);
		DrawRectangleLinesEx(wave.bounds, 3.0f, kShockwaveEdgeColor);

		const float circleRadius = wave.bounds.height * 0.8f;
		const int circleX = static_cast<int>(wave.bounds.x);
		const int circleY = static_cast<int>(wave.bounds.y + wave.bounds.height * 0.5f);
		Color frontColor = kShockwaveEdgeColor;
		frontColor.a = static_cast<unsigned char>(static_cast<float>(frontColor.a) * alphaFactor);
		DrawCircleGradient(circleX, circleY, circleRadius, frontColor, ColorAlpha(frontColor, 0.0f));

		Rectangle dust{
			wave.bounds.x + wave.bounds.width * 0.1f,
			wave.bounds.y + wave.bounds.height * 0.65f,
			wave.bounds.width * 0.8f,
			wave.bounds.height * 0.35f
		};
		Color dustColor = kShockwaveDustColor;
		dustColor.a = static_cast<unsigned char>(static_cast<float>(dustColor.a) * alphaFactor);
		DrawRectangleRec(dust, dustColor);
	}
}

void BossState::drawBossHp() const {
	const int hp = m_boss.health();
	const int maxHp = std::max(1, m_boss.maxHealth());
	const float ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
	const float barWidth = kBossHpBarWidth;
	const float x = static_cast<float>(GetScreenWidth()) - barWidth - kUiPadding;
	const float y = static_cast<float>(kUiPadding);
	DrawText(TextFormat("Boss HP %d / %d", hp, maxHp), static_cast<int>(x), static_cast<int>(y - 22.0f), kBossHpFontSize, kBossHpTextColor);
	DrawRectangleRec(Rectangle{x, y, kBossHpBarWidth, kBossHpBarHeight}, kBossHpBarBg);
	DrawRectangleRec(Rectangle{x, y, kBossHpBarWidth * ratio, kBossHpBarHeight}, kBossHpBarFill);
	DrawRectangleLines(static_cast<int>(x), static_cast<int>(y), static_cast<int>(kBossHpBarWidth), static_cast<int>(kBossHpBarHeight), kBossHpBarOutline);
}

void BossState::drawPlayerHp() const {
	const int hp = m_player.health();
	const int maxHp = std::max(1, m_player.maxHealth());
	const float ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
	const float barWidth = kPlayerHpBarWidth;
	const float barHeight = kPlayerHpBarHeight;
	const float x = static_cast<float>(kUiPadding);
	const float y = static_cast<float>(kUiPadding);
	DrawText("Player HP", static_cast<int>(x), static_cast<int>(y - 18.0f), kPlayerHpFontSize, kPlayerHpLabelColor);
	DrawRectangleRec(Rectangle{x, y, barWidth, barHeight}, kPlayerHpBarBg);
	DrawRectangleRec(Rectangle{x, y, barWidth * ratio, barHeight}, kPlayerHpBarFill);
	DrawRectangleLines(static_cast<int>(x), static_cast<int>(y), static_cast<int>(barWidth), static_cast<int>(barHeight), kPlayerHpBarOutline);
}


void BossState::drawGround() const {
    //std::cout << "Draw Ground!" << std::endl;
    const Rectangle soil{0.0f, m_groundY, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) - m_groundY};
 //   DrawRectangleRec(soil, kSoilColor);
    DrawRectangle(0, static_cast<int>(m_groundY) - kGroundOffset, GetScreenWidth(), kGroundThickness, kGroundColor);
}


} // namespace ks
