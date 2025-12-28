#include "Enemy.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

namespace ks {

namespace {
constexpr float kInitialSpawnX = 100.0f;
constexpr Color kEnemyFallbackColor{189, 66, 66, 255};
constexpr float kMovementThreshold = 0.5f;
constexpr float kSpriteHeightScale = 3.2f;
constexpr float kHalf = 0.5f;
} // namespace

// 共有スプライトをロードし参照カウントを増やす
void Enemy::AcquireSpriteSheet(const char* path) {

    ++s_spriteUsers;
}

// 参照カウントを減らし、誰も使わなければ解放する
void Enemy::ReleaseSpriteSheet() {
    if (s_spriteUsers == 0) {
        return;
    }
    --s_spriteUsers;
}

// 位置・体力・見た目・ステートマシンを初期化する
Enemy::Enemy() {
    m_position.set(kInitialSpawnX, m_groundY);
    m_health.setMax(kDefaultMaxHealth);
    m_render.setFallback(kEnemyFallbackColor);
    resetStateMachine();
}

// ムーブ時にも必要な情報を引き継ぎ、ステートマシンを再紐付けする
Enemy::Enemy(Enemy&& other) noexcept
    : m_position(std::move(other.m_position))
    , m_render(std::move(other.m_render))
    , m_health(std::move(other.m_health))
    , m_stateMachine(std::move(other.m_stateMachine))
    , m_chaseState(std::move(other.m_chaseState))
    , m_speed(other.m_speed)
    , m_radius(other.m_radius)
    , m_groundY(other.m_groundY)
    , m_minX(other.m_minX)
    , m_maxX(other.m_maxX)
    , m_patrolDirection(other.m_patrolDirection)
    , m_animTimer(other.m_animTimer)
    , m_isMoving(other.m_isMoving)
    , m_facingLeft(other.m_facingLeft)
    , m_isAttacking(other.m_isAttacking)
    , m_attackRange(other.m_attackRange)
    , m_attackCooldown(other.m_attackCooldown)
    , m_attackTimer(other.m_attackTimer)
    , m_attackTriggered(other.m_attackTriggered) {
    resetStateMachine();
}

// 代入でも同様に情報を移し替え、ステートマシンを再設定する
Enemy& Enemy::operator=(Enemy&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    m_position = std::move(other.m_position);
    m_render = std::move(other.m_render);
    m_health = std::move(other.m_health);
    m_stateMachine = std::move(other.m_stateMachine);
    m_chaseState = std::move(other.m_chaseState);
    m_speed = other.m_speed;
    m_radius = other.m_radius;
    m_groundY = other.m_groundY;
    m_minX = other.m_minX;
    m_maxX = other.m_maxX;
    m_patrolDirection = other.m_patrolDirection;
    m_animTimer = other.m_animTimer;
    m_isMoving = other.m_isMoving;
    m_facingLeft = other.m_facingLeft;
    m_isAttacking = other.m_isAttacking;
    m_attackRange = other.m_attackRange;
    m_attackCooldown = other.m_attackCooldown;
    m_attackTimer = other.m_attackTimer;
    m_attackTriggered = other.m_attackTriggered;

    resetStateMachine();
    return *this;
}

// 現在のステートに行動を委譲する
void Enemy::update(float dt) {
    m_stateMachine.update(dt);
}

// スプライトが使えれば優先し、無ければフォールバックを描く
void Enemy::render() {
    drawFallback();

}

// 絶対座標を設定し、地面高さも更新する
void Enemy::setPosition(float x, float y) {
    m_position.set(x, y);
    m_groundY = y;

    const float laneCenter = (m_minX + m_maxX) * kHalf;
    m_patrolDirection = (x >= laneCenter) ? -1.0f : 1.0f;
}

// X 方向だけ平行移動し、Y は地面上に固定する
void Enemy::translate(float dx, float) {
    m_position.translate(dx, 0.0f);
    m_position.set(m_position.x(), m_groundY);
}

// 位置ベクトルを返す
Vector2 Enemy::getPosition() const {
    return m_position.toVector();
}

// 当たり半径を返す
float Enemy::radius() const {
    return m_radius;
}

// 接地高さを揃える
void Enemy::setGround(float y) {
    m_groundY = y;
    m_position.set(m_position.x(), m_groundY);
}

// 横移動可能範囲を記録し、はみ出しを補正する
void Enemy::setMovementBounds(float minX, float maxX) {
    m_minX = std::min(minX, maxX);
    m_maxX = std::max(minX, maxX);
    clampToBounds();
}

// 1 フレームの攻撃イベントを消費する
bool Enemy::consumeAttackEvent() {
    if (m_attackTriggered) {
        m_attackTriggered = false;
        return true;
    }
    return false;
}

// X 座標を移動範囲に収める
void Enemy::clampToBounds() {
    const float clampedX = std::max(m_minX + m_radius, std::min(m_maxX - m_radius, m_position.x()));
    m_position.set(clampedX, m_groundY);
}

// 前フレームとの移動量から向きや移動フラグを決める
void Enemy::updateAnimationState(float prevX) {
    const float delta = m_position.x() - prevX;
    m_isMoving = std::fabs(delta) > kMovementThreshold;
    m_facingLeft = delta < 0.0f;
    
}


// テクスチャが無いときの簡易描画
void Enemy::drawFallback() const {
//    m_render.draw(m_position.toVector(), m_radius, Color{200, 80, 80, 255});
}

// ステートマシンの所有者と現在状態を再紐付けする
void Enemy::resetStateMachine() {
    m_stateMachine.setOwner(this);
    m_stateMachine.overrideState(&m_chaseState, true);
}

} // namespace ks
