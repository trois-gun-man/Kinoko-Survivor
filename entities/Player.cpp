#include "Player.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace ks {

namespace {
constexpr float kInitialSpawnX = 400.0f;
constexpr Color kPlayerFallbackColor{245, 222, 179, 255};
constexpr Color kPlayerDrawColor{250, 250, 250, 255};
constexpr Color kPlayerAttackColor{250, 0, 250, 255};
constexpr const char* kPlayerSpriteSheetPath = "assets/player_sprite.png";
constexpr int kPlayerSpriteColumns = 3;
constexpr int kPlayerSpriteRows = 3;
constexpr float kMoveFrameDuration = 0.12f;
constexpr float kJumpFrameDuration = 0.14f;
constexpr float kAttackFrameDuration = 0.09f;
constexpr float kSpriteHeightScale = 2.4f;
constexpr float kSpriteSizeMultiplier = 1.5f;

struct AnimationConfig {
	int row = 0;
	int frames = 1;
	float duration = 0.0f;
};

constexpr AnimationConfig kAnimationConfigs[] = {
    {2, 1, 0.0f},                 // Idle
    {2, kPlayerSpriteColumns, kMoveFrameDuration},   // Move
    {0, 2, kJumpFrameDuration},   // Jump
    {1, kPlayerSpriteColumns, kAttackFrameDuration}  // Attack
};
} // namespace

// 位置・体力・ステートマシンを初期化する
Player::Player() {
    m_position.set(kInitialSpawnX, m_groundY);
    m_health.setMax(kDefaultMaxHealth);
    m_render.setFallback(kPlayerFallbackColor);
    m_stateMachine.setOwner(this);
    m_stateMachine.changeState(&m_moveState);
    loadSpriteSheet();
}

Player::~Player() {
	unloadSpriteSheet();
}

void Player::update(float dt) {
    // 水平移動はステートマシンに任せる
    m_stateMachine.update(dt);

    // 垂直移動の計算
    const float gravityUp   = 2000.0f;
    const float gravityDown = 2000.0f;
    if (velocityY < 0.0f) {
        
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

	updateAnimation(dt);
}


// 色付きの丸でプレイヤーを描画する
void Player::render() {
    if (m_hasSpriteSheet) {
		drawSprite();
		return;
	}
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
int Player::getAttackPower() const {
    return m_attackPower;
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
    m_moveInput = direction;
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
    if (!m_isAttacking) {
        return;
    }

    Vector2 effectCenter = m_position.toVector();
    effectCenter.x += (m_facing == Facing::Left ? -m_radius.x : m_radius.x) * 0.6f;
    effectCenter.y += m_radius.y * 0.5f;
    DrawCircleGradient(
        static_cast<int>(effectCenter.x),
        static_cast<int>(effectCenter.y),
        m_radius.x * 0.9f,
        Color{255, 255, 255, 90},
        Color{255, 255, 255, 0}
    );

    if (!m_hasSpriteSheet) {
        Vector2 attackPos = m_position.toVector();
        if (m_facing == Facing::Left) {
            attackPos.x -= m_radius.x;
        } else {
            attackPos.x += m_radius.x;
        }
        float attackRadius = m_radius.x;
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
    if (m_hasSpriteSheet) {
		m_animState = AnimationState::Attack;
		m_currentFrame = 0;
		m_animTimer = 0.0f;
	}
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
        m_radius.x*3.0f,
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

void Player::loadSpriteSheet() {
    if (!FileExists(kPlayerSpriteSheetPath)) {
        m_hasSpriteSheet = false;
        return;
    }

    m_spriteSheet = LoadTexture(kPlayerSpriteSheetPath);
    m_hasSpriteSheet = m_spriteSheet.id != 0;
    if (m_hasSpriteSheet) {
        m_frameWidth = m_spriteSheet.width / kPlayerSpriteColumns;
        m_frameHeight = m_spriteSheet.height / kPlayerSpriteRows;
    }
}

void Player::unloadSpriteSheet() {
    if (!m_hasSpriteSheet) {
        return;
    }
    UnloadTexture(m_spriteSheet);
    m_spriteSheet = Texture2D{};
    m_hasSpriteSheet = false;
    m_frameWidth = 0;
    m_frameHeight = 0;
}

Player::AnimationState Player::calculateAnimationState() const {
    if (!isOnGround()) {
        return AnimationState::Jump;
    }
    if (m_isAttacking) {
        return AnimationState::Attack;
    }
    if (std::fabs(m_moveInput) > 0.05f) {
        return AnimationState::Move;
    }
    return AnimationState::Idle;
}

void Player::updateAnimation(float dt) {
    if (!m_hasSpriteSheet || m_frameWidth == 0 || m_frameHeight == 0) {
        return;
    }

    const AnimationState nextState = calculateAnimationState();
    if (nextState != m_animState) {
        m_animState = nextState;
        m_currentFrame = 0;
        m_animTimer = 0.0f;
    }

    const AnimationConfig& config = kAnimationConfigs[static_cast<int>(m_animState)];
    if (config.frames <= 1 || config.duration <= 0.0f) {
        m_currentFrame = 0;
        return;
    }

    // Jump animation 固定フレーム: 着地（地上）は 0、それ以外は 1
    if (m_animState == AnimationState::Jump) {
        m_currentFrame = isOnGround() ? 0 : 1;
        return;
    }

    m_animTimer += dt;
    if (m_animTimer >= config.duration) {
        m_animTimer = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % config.frames;
    }
}

void Player::drawSprite() const {
    if (!m_hasSpriteSheet || m_frameWidth == 0 || m_frameHeight == 0) {
        return;
    }

    const AnimationConfig& config = kAnimationConfigs[static_cast<int>(m_animState)];
    const int frameIndex = std::min(m_currentFrame, std::max(1, config.frames) - 1);
    Rectangle src{
        static_cast<float>(m_frameWidth) * static_cast<float>(frameIndex),
        static_cast<float>(m_frameHeight) * static_cast<float>(config.row),
        static_cast<float>(m_frameWidth),
        static_cast<float>(m_frameHeight)
    };
    if (m_facing == Facing::Left) {
        src.width = -src.width;
    }

    const Vector2 pos = m_position.toVector();
    const float bboxWidth = m_radius.x;
    const float bboxHeight = m_radius.y;
    const float centerX = pos.x + bboxWidth * 0.5f;
    const float bottomY = pos.y + bboxHeight;
    const float scale = (bboxHeight * kSpriteHeightScale) / static_cast<float>(m_frameHeight);
    const float width = static_cast<float>(m_frameWidth) * scale * kSpriteSizeMultiplier;
    const float height = static_cast<float>(m_frameHeight) * scale * kSpriteSizeMultiplier;
    const Rectangle dest{
        centerX - width * 0.5f,
        bottomY - height*0.5f-10.0f,
        width,
        height
    };

    DrawTexturePro(m_spriteSheet, src, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}
    
} // namespace ks
