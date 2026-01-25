#include "Boss.hpp"

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
    updateAnimation(dt);
}

void Boss::render() {
    if (isDead()) {
        return;
    }

    Vector2 drawPos = m_position.toVector();
    drawPos.y -= 80.0f; // Enemyと同じ補正
    drawPos.x -= 10.0f; // 中心合わせ
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
}

void Boss::setGround(float y) {
    m_groundY = y;
    m_position.set(m_position.x(), m_groundY);
}

Rectangle Boss::getHitBox() const {
    Vector2 pos = m_position.toVector();
    return Rectangle{
        pos.x,
        pos.y + m_radius.y,
        m_radius.x,
        m_radius.y
    };
}

void Boss::applyDamage(int damage) {
    m_health.applyDamage(damage);
}

bool Boss::isDead() const {
    return m_health.isDead();
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

void Boss::drawSprite(const Vector2& drawPos) const {
    if (!s_spriteLoaded) {
        return;
    }

    const float frameWidth = static_cast<float>(s_spriteSheet.width) / static_cast<float>(kSpriteColumns);
    const float frameHeight = static_cast<float>(s_spriteSheet.height) / static_cast<float>(kSpriteRows);
    const int column = m_currentFrame % kSpriteColumns;
    const int row = m_isJumping ? 1 : 0;

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
