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

// スプライトがあればテクスチャ描画、無ければ円形で代用する
void RenderComponent::draw(const Vector2& position, Vector2 size, Color tint) const {
    if (m_hasSprite) {
       // const float diameter = radius * 2.0f;
       // const Rectangle source{0.0f, 0.0f, static_cast<float>(m_sprite.width), static_cast<float>(m_sprite.height)};
       // const Rectangle dest{position.x - radius, position.y - radius, diameter, diameter};
       // const Vector2 origin{0.0f, 0.0f};
       // DrawTexturePro(m_sprite, source, dest, origin, 0.0f, tint);
       // return;
    }

    const Color color = (tint.a > 0) ? tint : m_fallbackColor;
    DrawRectangleV(position, size, color);
}

} // namespace ks
