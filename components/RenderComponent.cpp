#include "RenderComponent.hpp"

namespace ks {

// テクスチャを保持していれば破棄する
RenderComponent::~RenderComponent() {
    if (m_hasSprite) {
        UnloadTexture(m_sprite);
    }
}

// 指定テクスチャを読み込み、失敗した場合はフォールバックに切り替える
void RenderComponent::loadSprite(const char* path) {
    if (!path || !FileExists(path)) {
        m_hasSprite = false;
        return;
    }

    if (m_hasSprite) {
        UnloadTexture(m_sprite);
        m_hasSprite = false;
    }

    m_sprite = LoadTexture(path);
    m_hasSprite = m_sprite.id != 0;
}

// フォールバック時の塗りつぶし色を更新する
void RenderComponent::setFallback(Color color) {
    m_fallbackColor = color;
}

// 円形描画
void RenderComponent::draw(const Vector2& position, Vector2 size, Color tint) const {
    const Color color = (tint.a > 0) ? tint : m_fallbackColor;
    DrawRectangleV(position, size, color);
}

} // namespace ks
