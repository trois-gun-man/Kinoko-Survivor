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
    static constexpr int kMaxHealth = 30;
    static constexpr float kSpriteScale = 4.0f;
    static constexpr Vector2 kRadius = {60.0f * kSpriteScale, 48.0f * kSpriteScale}; // 大きめ
    static constexpr Color kBossColor{120, 40, 180, 255};
    static constexpr const char* kSpritePath = "assets/Boss_sprite.png";
    static constexpr int kSpriteColumns = 2;
    static constexpr int kSpriteRows = 2;
    static constexpr float kFrameDuration = 0.18f;

    Vector2 m_radius = kRadius;
    float m_groundY = 360.0f;
    float m_animTimer = 0.0f;
    int m_currentFrame = 0;
    bool m_isJumping = false;

    static inline Texture2D s_spriteSheet{};
    static inline bool s_spriteLoaded = false;
    static inline int s_spriteUsers = 0;

    static void AcquireSpriteSheet();
    static void ReleaseSpriteSheet();
    void updateAnimation(float dt);
    void drawSprite(const Vector2& drawPos) const;
};

} // namespace ks
