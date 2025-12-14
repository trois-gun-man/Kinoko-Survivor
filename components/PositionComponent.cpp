#include "PositionComponent.hpp"

namespace ks {

// 指定座標で位置を初期化・更新する
void PositionComponent::set(float x, float y) {
    m_x = x;
    m_y = y;
}

// 与えられた差分を現在位置へ加算する
void PositionComponent::translate(float dx, float dy) {
    m_x += dx;
    m_y += dy;
}

// raylib のベクトル型として現在位置を返す
Vector2 PositionComponent::toVector() const {
    return Vector2{m_x, m_y};
}

// X のみ返す
float PositionComponent::x() const {
    return m_x;
}

// Y のみ返す
float PositionComponent::y() const {
    return m_y;
}

} // namespace ks
