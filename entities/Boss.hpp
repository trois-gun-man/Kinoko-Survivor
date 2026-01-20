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

    void update(float dt) override;
    void render() override;

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
    static constexpr Vector2 kRadius = {60.0f, 48.0f}; // 大きめ
    static constexpr Color kBossColor{120, 40, 180, 255};

    Vector2 m_radius = kRadius;
    float m_groundY = 360.0f;
};

} // namespace ks
