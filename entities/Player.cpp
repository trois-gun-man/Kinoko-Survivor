#include "Player.hpp"

#include <algorithm>
#include <iostream>

namespace ks {

namespace {
constexpr float kInitialSpawnX = 400.0f;
constexpr Color kPlayerFallbackColor{245, 222, 179, 255};
constexpr Color kPlayerDrawColor{250, 250, 250, 255};
constexpr Color kPlayerAttackColor{250, 0, 250, 255};
} // namespace

// 位置・体力・ステートマシンを初期化する
Player::Player() {
    m_position.set(kInitialSpawnX, m_groundY);
    m_health.setMax(kDefaultMaxHealth);
    m_render.setFallback(kPlayerFallbackColor);
    m_stateMachine.setOwner(this);
    m_stateMachine.changeState(&m_moveState);
}

void Player::update(float dt) {
    // 水平移動はステートマシンに任せる
    m_stateMachine.update(dt);

    // 垂直移動の計算
    const float gravityUp   = 2500.0f;
    const float gravityDown = 2500.0f;
    if (velocityY < 0.0f) {
        if (velocityY > -800.0f) {
            velocityY *= 0.98f; // ジャンプの頂点付近で減速
        }
        velocityY += gravityUp * dt; // 上昇中の重力
    } else {
        velocityY += gravityDown * dt; // 重力加速度
    }
    m_position.translate(0.0f, velocityY * dt);

    // 地面との衝突判定
    if (m_position.y() >= m_groundY) {
        m_position.set(m_position.x(), m_groundY);
        velocityY = 0.0f;
    }

    //  std::cout << "y=" << m_position.y()
    //  << " move=" << velocityY * dt
    //  << std::endl;

    if (m_isAttacking) {
        if (m_facing != m_attackFacing) {
            m_isAttacking = false; // 向きが変わったら攻撃キャンセル
            m_attackTimer = 0.0f;
            return;
        }
        m_attackTimer -= dt;
        if (m_attackTimer <= 0.0f) { // 攻撃状態を一定時間維持
            m_isAttacking = false;
            m_attackHit = false;
        }
    //    std::cout << "dt=" << dt << " attackTimer=" << m_attackTimer << std::endl;
    }
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
Vector2 Player::radius() const {
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
    if (direction < 0.0f) {
        m_facing = Facing::Left;
    } else if (direction > 0.0f) {
        m_facing = Facing::Right;
    }
    m_position.translate(direction * m_speed * dt, 0.0f);
    clampToBounds();
}
bool Player::isOnGround() const {
    return m_position.y() >= m_groundY && velocityY >= 0.0f;
}

void Player::jump() {
    if (!isOnGround()) return;
    velocityY = -700.0f;
}

void Player::draw() {
    if (m_isAttacking){
        Vector2 attackPos = m_position.toVector();
        if (m_facing == Facing::Left) {
            attackPos.x -= m_radius.x; // 左向きなら左にオフセット
        } else {
            attackPos.x += m_radius.x; // 右向きなら右にオフセット
        }
        float attackRadius = m_radius.x /* * 0.5f */; // 攻撃判定は半径を小さくする
        m_render.draw(attackPos, {attackRadius, m_radius.y}, kPlayerAttackColor);  
    }
}

void Player::setFirstAttack() {
    if (first_attack == false){
      first_attack = true;
    }
}

bool Player::getFirstAttack() {
    return first_attack;
}

void Player::attack() {
    m_isAttacking = true;
    m_attackTimer = 0.3f; // 攻撃状態を維持する時間
    m_attackFacing = m_facing;
    m_attackHit = false;
    if (first_attack) {
        first_attack = false;
    }
}

StateMachine& Player::getStateMachine() {
    return m_stateMachine;
}

PlayerAttackState& Player::getAttackState() {
    return m_attackState;
}

PlayerMoveState& Player::getMoveState() {
    return m_moveState;
}

// 移動範囲からはみ出さないように座標を調整する
void Player::clampToBounds() {
    const Vector2 pos = m_position.toVector();
    const float clampedX = std::max(m_minX, std::min(m_maxX - m_radius.x, pos.x));
    m_position.set(clampedX, pos.y);
}

Rectangle Player::getAttackHitBox() const {
    if (!m_isAttacking) {
        return Rectangle{0, 0, 0, 0}; // 攻撃していない場合は空の矩形を返す
    }

    Vector2 attackPos = m_position.toVector();
    if (m_facing == Facing::Left) {
        attackPos.x -= m_radius.x; // 左向きなら左にオフセット
    } else {
        attackPos.x += m_radius.x; // 右向きなら右にオフセット
    }
    float attackRadius = m_radius.x /* * 0.5f */; // 攻撃判定は半径を小さくする

    return Rectangle{
        attackPos.x,
        attackPos.y,
        m_radius.x,
        m_radius.y
    };
}

bool Player::isAttacking() const {
    return m_isAttacking;
}

void Player::setAttackHit(bool hit) {
    m_attackHit = hit;
}

bool Player::getAttackHit() const {
    return m_attackHit;
}
    
} // namespace ks
