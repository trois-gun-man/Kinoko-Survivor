#include "Boss.hpp"

namespace ks {

Boss::Boss() {
    m_health.setMax(kMaxHealth);
    m_render.setFallback(kBossColor);
}

void Boss::update(float) {
    // 今は何もしない（固定ボス）
}

void Boss::render() {
    if (isDead()) {
        return;
    }

    Vector2 drawPos = m_position.toVector();
    drawPos.y += 18.0f; // Enemyと同じ補正

    m_render.draw(drawPos, m_radius, kBossColor);
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
      //  pos.y + m_radius.y,
        pos.y + 18.0f, // Enemyと同じ補正
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

} // namespace ks
