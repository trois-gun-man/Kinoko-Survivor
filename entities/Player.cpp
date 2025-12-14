#include "Player.hpp"

#include <algorithm>

namespace ks {

namespace {
constexpr float kInitialSpawnX = 400.0f;
constexpr Color kPlayerFallbackColor{245, 222, 179, 255};
constexpr Color kPlayerDrawColor{250, 250, 250, 255};
} // namespace

// 位置・体力・ステートマシンを初期化する
Player::Player() {
    m_position.set(kInitialSpawnX, m_groundY);
    m_health.setMax(kDefaultMaxHealth);
    m_render.setFallback(kPlayerFallbackColor);
    m_stateMachine.setOwner(this);
    m_stateMachine.changeState(&m_moveState);
}

// 現在の状態ロジックを進める
void Player::update(float dt) {
    m_stateMachine.update(dt);
}

// 色付きの丸でプレイヤーを描画する
void Player::render() {
    m_render.draw(m_position.toVector(), m_radius, kPlayerDrawColor);
}

// ワールド座標を返す
Vector2 Player::getPosition() const {
    return m_position.toVector();
}

// 受け取った値だけ体力を減らす
void Player::applyDamage(int amount) {
    m_health.applyDamage(amount);
}

// 体力ゼロ以下を死亡と見なす
bool Player::isDead() const {
    return m_health.isDead();
}

// 当たり判定半径を返す
float Player::radius() const {
    return m_radius;
}

// 現在 HP を返す
int Player::health() const {
    return m_health.current();
}

// 最大 HP を返す
int Player::maxHealth() const {
    return m_health.max();
}

// 接地 Y を更新し、足元をそろえる
void Player::setGround(float y) {
    m_groundY = y;
    m_position.set(m_position.x(), m_groundY);
}

// 左右の移動制限を設定した上で位置を補正する
void Player::setMovementBounds(float minX, float maxX) {
    m_minX = std::min(minX, maxX);
    m_maxX = std::max(minX, maxX);
    clampToBounds();
}

// X を直接設定し、範囲内へ丸める
void Player::setPosition(float x) {
    m_position.set(x, m_groundY);
    clampToBounds();
}

// 入力方向を元に内部で移動計算と範囲補正を行う
void Player::moveHorizontally(float direction, float dt) {
    m_position.translate(direction * m_speed * dt, 0.0f);
    clampToBounds();
}

// 移動範囲からはみ出さないように座標を調整する
void Player::clampToBounds() {
    const Vector2 pos = m_position.toVector();
    const float clampedX = std::max(m_minX + m_radius, std::min(m_maxX - m_radius, pos.x));
    m_position.set(clampedX, m_groundY);
}

} // namespace ks
