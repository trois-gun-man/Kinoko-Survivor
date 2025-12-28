#include "HealthComponent.hpp"

#include <algorithm>

namespace ks {

// 最大値を正規化して反映し、現在値も即座に満タンへ合わせる
void HealthComponent::setMax(int value) {
    m_max = std::max(0, value);
    m_current = m_max;
}

// ゼロ未満にならないようにしながら一定量のダメージを与える
void HealthComponent::applyDamage(int value) {
    m_current = std::max(0, m_current - std::max(0, value));
}

// 残り体力がゼロ以下なら死亡扱いとする
bool HealthComponent::isDead() const {
    return m_current <= 0;
}

// 現在値をそのまま返す
int HealthComponent::current() const {
    return m_current;
}

// 最大値をそのまま返す
int HealthComponent::max() const {
    return m_max;
}

} // namespace ks
