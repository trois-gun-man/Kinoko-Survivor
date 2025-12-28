#include "RenderComponent.hpp"

namespace ks {


// フォールバック時の塗りつぶし色を更新する
void RenderComponent::setFallback(Color color) {
    m_fallbackColor = color;
}

// 円形描画
void RenderComponent::draw(const Vector2& position, float radius, Color tint) const {
    const Color color = (tint.a > 0) ? tint : m_fallbackColor;
    DrawRectangleV(position, size, color);
}

} // namespace ks
