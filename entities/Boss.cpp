#include "Boss.hpp"
#include <iostream>
namespace ks {

Boss::Boss() {
    m_health.setMax(kMaxHealth);
    m_render.setFallback(kBossColor);
    AcquireSpriteSheet();
}

Boss::~Boss() {
    ReleaseSpriteSheet();
}

void Boss::update(float dt) {
    if (isDead()) {
        return;
    }

    updateJump(dt);
    updateAnimation(dt);

    if (m_landingFrameTimer > 0.0f) {
        m_landingFrameTimer -= dt;
        if (m_landingFrameTimer < 0.0f) {
            m_landingFrameTimer = 0.0f;
        }
    }
}

void Boss::render() {
    if (isDead()) {
        return;
    }

    Vector2 drawPos = m_position.toVector();
    drawPos.x += kSpriteOffset.x;
    drawPos.y += kSpriteOffset.y;
    if (s_spriteLoaded) {
        drawSprite(drawPos);
        return;
    }

    m_render.draw(drawPos, m_radius, kBossColor);
}

void Boss::setJumping(bool jumping) {
    m_isJumping = jumping;
}

void Boss::setPosition(float x, float y) {
    m_position.set(x, y);
    m_groundY = y;
    m_isAirborne = false;
    m_verticalVelocity = 0.0f;
    m_jumpTimer = kJumpInterval;
    m_pendingShockwave = false;
    m_landingFrameTimer = 0.0f;
}

void Boss::setGround(float y) {
    m_groundY = y;
    m_position.set(m_position.x(), m_groundY);
    m_isAirborne = false;
    m_verticalVelocity = 0.0f;
    m_jumpTimer = kJumpInterval;
    m_pendingShockwave = false;
    m_landingFrameTimer = 0.0f;
}

Rectangle Boss::getHitBox() const {
    Vector2 topLeft = m_position.toVector();
    topLeft.x += kSpriteOffset.x;
    topLeft.y += kSpriteOffset.y;

    float width = m_radius.x - kHitboxInset.x;
    float height = m_radius.y - kHitboxInset.y;
    if (width < 0.0f) {
        width = 0.0f;
    }
    if (height < 0.0f) {
        height = 0.0f;
    }
    std::cout << "Boss HitBox: x=" << topLeft.x + kHitboxInset.x * 0.5f
              << " y=" << topLeft.y + kHitboxInset.y * 0.5f
              << " w=" << width
              << " h=" << height << std::endl;
    return Rectangle{
        topLeft.x+ kHitboxInset.x,
        topLeft.y,
        width*0.5f,
        height
    };
}

void Boss::applyDamage(int damage) {
    m_health.applyDamage(damage);
}

bool Boss::isDead() const {
    return m_health.isDead();
}

Vector2 Boss::getPosition() const {
    return m_position.toVector();
}

Vector2 Boss::radius() const {
    return m_radius;
}

int Boss::health() const {
    return m_health.current();
}

int Boss::maxHealth() const {
    return m_health.max();
}

bool Boss::consumeShockwaveEvent() {
    if (m_pendingShockwave) {
        m_pendingShockwave = false;
        return true;
    }
    return false;
}

void Boss::AcquireSpriteSheet() {
    if (s_spriteUsers == 0) {
        if (FileExists(kSpritePath)) {
            s_spriteSheet = LoadTexture(kSpritePath);
            s_spriteLoaded = s_spriteSheet.id != 0;
        } else {
            s_spriteLoaded = false;
        }
    }
    ++s_spriteUsers;
}

void Boss::ReleaseSpriteSheet() {
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

void Boss::updateAnimation(float dt) {
    if (!s_spriteLoaded) {
        return;
    }

    m_animTimer += dt;
    if (m_animTimer >= kFrameDuration) {
        m_animTimer -= kFrameDuration;
        m_currentFrame = (m_currentFrame + 1) % kSpriteColumns;
    }
}

void Boss::updateJump(float dt) {
    if (m_isAirborne) {
        m_verticalVelocity += kJumpGravity * dt;
        m_position.translate(0.0f, m_verticalVelocity * dt);

        if (m_position.y() >= m_groundY) {
            m_position.set(m_position.x(), m_groundY);
            m_verticalVelocity = 0.0f;
            m_isAirborne = false;
            m_isJumping = false;
            m_jumpTimer = kJumpInterval;
            m_pendingShockwave = true;
            m_landingFrameTimer = kLandingFrameDuration;
        }
        return;
    }

    m_jumpTimer -= dt;
    if (m_jumpTimer <= 0.0f) {
        m_isAirborne = true;
        m_isJumping = true;
        m_verticalVelocity = kJumpVelocity;
        m_jumpTimer = 0.0f;
        m_landingFrameTimer = 0.0f;
    }
}

void Boss::drawSprite(const Vector2& drawPos) const {
    if (!s_spriteLoaded) {
        return;
    }

    const float frameWidth = static_cast<float>(s_spriteSheet.width) / static_cast<float>(kSpriteColumns);
    const float frameHeight = static_cast<float>(s_spriteSheet.height) / static_cast<float>(kSpriteRows);
    int column = m_currentFrame % kSpriteColumns;
    int row = 0;

    if (m_landingFrameTimer > 0.0f) {
        row = 1;
        column = 1;
    } else if (m_isJumping) {
        row = 1;
        column = 0;
    }

    const Rectangle src{
        frameWidth * static_cast<float>(column),
        frameHeight * static_cast<float>(row),
        frameWidth,
        frameHeight
    };

    const Rectangle dest{drawPos.x, drawPos.y, m_radius.x, m_radius.y};
    const Vector2 origin{0.0f, 0.0f};

    DrawTexturePro(s_spriteSheet, src, dest, origin, 0.0f, WHITE);
}

} // namespace ks
