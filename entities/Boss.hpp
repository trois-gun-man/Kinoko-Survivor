#pragma once

#include <raylib.h>

#include "Entity.hpp"
#include "../components/HealthComponent.hpp"
#include "../components/PositionComponent.hpp"
#include "../components/RenderComponent.hpp"

namespace ks {

class Boss : public Entity {
public:
    Boss();
    ~Boss();
    Boss(const Boss&) = delete;
    Boss& operator=(const Boss&) = delete;
    Boss(Boss&&) = delete;
    Boss& operator=(Boss&&) = delete;

    void update(float dt) override;
    void render() override;
    void setJumping(bool jumping);

    [[nodiscard]] Vector2 getPosition() const;
    [[nodiscard]] Vector2 radius() const;
    [[nodiscard]] int health() const;
    [[nodiscard]] int maxHealth() const;
    bool consumeShockwaveEvent();

    void setPosition(float x, float y);
    void setGround(float y);

    Rectangle getHitBox() const;

    void applyDamage(int damage);
    bool isDead() const;

private:
    PositionComponent m_position;
    RenderComponent m_render;
    HealthComponent m_health;

    // ボス用パラメータ
    static constexpr int kMaxHealth = 100;
    static constexpr float kSpriteScale = 4.0f;
    static constexpr Vector2 kRadius = {60.0f * kSpriteScale, 48.0f * kSpriteScale}; // 大きめ
    static constexpr Vector2 kSpriteOffset{-10.0f, -80.0f};
    static constexpr Vector2 kHitboxInset{60.0f, 20.0f};
    static constexpr Color kBossColor{120, 40, 180, 255};
    static constexpr const char* kSpritePath = "assets/Boss_sprite.png";
    static constexpr int kSpriteColumns = 2;
    static constexpr int kSpriteRows = 2;
    static constexpr float kFrameDuration = 1.8f;
    static constexpr float kJumpInterval = 3.0f;
    static constexpr float kJumpGravity = 2600.0f;
    static constexpr float kJumpVelocity = -900.0f;
    static constexpr float kLandingFrameDuration = 0.35f;

    Vector2 m_radius = kRadius;
    float m_groundY = 360.0f;
    float m_animTimer = 0.0f;
    int m_currentFrame = 0;
    bool m_isJumping = false;
    float m_jumpTimer = kJumpInterval;
    float m_verticalVelocity = 0.0f;
    bool m_isAirborne = false;
    bool m_pendingShockwave = false;
    float m_landingFrameTimer = 0.0f;

    static inline Texture2D s_spriteSheet{};
    static inline bool s_spriteLoaded = false;
    static inline int s_spriteUsers = 0;

    static void AcquireSpriteSheet();
    static void ReleaseSpriteSheet();
    void updateAnimation(float dt);
    void updateJump(float dt);
    void drawSprite(const Vector2& drawPos) const;
};

} // namespace ks
