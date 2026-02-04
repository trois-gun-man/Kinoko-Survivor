#include "Enemy.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

#include "Player.hpp"

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
    if (s_spriteUsers == 0) {
        if (FileExists(path)) {
            s_spriteSheet = LoadTexture(path);
            s_spriteLoaded = s_spriteSheet.id != 0;
        } else {
            s_spriteLoaded = false;
        }
    }
    ++s_spriteUsers;
}

// 参照カウントを減らし、誰も使わなければ解放する
void Enemy::ReleaseSpriteSheet() {
    if (s_spriteUsers == 0) {
        return;
    }

    --s_spriteUsers;
    if (s_spriteUsers == 0 && s_spriteLoaded) {
        UnloadTexture(s_spriteSheet);
        s_spriteSheet = Texture2D{};
        s_spriteLoaded = false;
    }
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
    , m_strategy(std::move(other.m_strategy))
    , m_stateMachine(std::move(other.m_stateMachine))
    , m_chaseState(std::move(other.m_chaseState))
    , m_target(other.m_target)
    , m_speed(other.m_speed)
    , m_radius(other.m_radius)
    , m_groundY(other.m_groundY)
    , m_minX(other.m_minX)
    , m_maxX(other.m_maxX)
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
    m_strategy = std::move(other.m_strategy);
    m_stateMachine = std::move(other.m_stateMachine);
    m_chaseState = std::move(other.m_chaseState);
    m_target = other.m_target;
    m_speed = other.m_speed;
    m_radius = other.m_radius;
    m_groundY = other.m_groundY;
    m_minX = other.m_minX;
    m_maxX = other.m_maxX;
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
    if (s_spriteLoaded) {
        drawSprite();
    } else {
        drawFallback();
    }
}

// 絶対座標を設定し、地面高さも更新する
void Enemy::setPosition(float x, float y) {
    m_position.set(x, y);
    m_groundY = y;
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

// 追うプレイヤーを設定する
void Enemy::setTarget(const Player* player) {
    m_target = player;
}

// 追跡対象がいるかを返す
bool Enemy::hasTarget() const {
    return m_target != nullptr;
}

// 追跡対象の座標、いなければ自身の座標を返す
Vector2 Enemy::getTargetPosition() const {
    if (m_target) {
        return m_target->getPosition();
    }
    return m_position.toVector();
}

// 行動ロジックで使う移動速度
float Enemy::getSpeed() const {
    return m_speed;
}

// 実装済み戦略だけを受け付けて差し替える
void Enemy::setStrategy(std::unique_ptr<AIStrategy> strategy) {
    if (strategy) {
        m_strategy = std::move(strategy);
    }
}

// 当たり半径を返す
Vector2 Enemy::radius() const {
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
    const Vector2 pos = m_position.toVector();
    const float clampedX = std::max(m_minX, std::min(m_maxX - m_radius.x, pos.x));
    m_position.set(clampedX, m_groundY);
}

// 前フレームとの移動量から向きや移動フラグを決める
void Enemy::updateAnimationState(float prevX) {
    const float delta = m_position.x() - prevX;
    m_isMoving = std::fabs(delta) > kMovementThreshold;

    if (hasTarget()) {
        m_facingLeft = (getTargetPosition().x < m_position.x());
    } else if (m_isMoving) {
        m_facingLeft = delta < 0.0f;
    }
}

// アニメーションフレームを計算してスプライトを描画する
void Enemy::drawSprite() const {
    const float frameWidth = static_cast<float>(s_spriteSheet.width) / static_cast<float>(s_spriteColumns);
    const float frameHeight = static_cast<float>(s_spriteSheet.height) / static_cast<float>(s_spriteRows);
    const int currentFrame = static_cast<int>(m_animTimer / s_frameDuration) % s_spriteColumns;
    const int row = m_isAttacking ? 2 : (m_isMoving ? 1 : 0);

    Rectangle src{frameWidth * currentFrame, frameHeight * row, frameWidth, frameHeight};
    if (m_facingLeft) {
        src.x += frameWidth;
        src.width = -frameWidth;
    }

    const float desiredHeight = m_radius.y * 2.0f * kSpriteHeightScale;
    const float scale = desiredHeight / frameHeight;
    const float width = frameWidth * scale;
    const float height = frameHeight * scale;
    const Rectangle dest{m_position.x() - width * kHalf, m_groundY - height* kHalf, width, height};
    DrawTexturePro(s_spriteSheet, src, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

// テクスチャが無いときの簡易描画
void Enemy::drawFallback() const {
    Vector2 drawPos = m_position.toVector();
    drawPos.y += 18.0f; // スプライト位置合わせ用補正
    m_render.draw(drawPos, m_radius, Color{200, 80, 80, 255});
}

// ステートマシンの所有者と現在状態を再紐付けする
void Enemy::resetStateMachine() {
    m_stateMachine.setOwner(this);
    m_stateMachine.overrideState(&m_chaseState, true);
}

Rectangle Enemy::getHitBox() const {
    Vector2 pos = m_position.toVector();
    return Rectangle{
        pos.x,
        pos.y + m_radius.y,
        m_radius.x,
        m_radius.y
    };
}

void Enemy::applyDamage(int damage) {
    m_health.applyDamage(damage);
}

bool Enemy::isDead() const {
    return m_health.isDead();
}

} // namespace ks
