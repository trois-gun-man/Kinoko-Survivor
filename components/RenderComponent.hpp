#pragma once

#include <raylib.h>

namespace ks {

// スプライト描画と単色フォールバックをまとめる表示用コンポーネント
class RenderComponent {
public:
    RenderComponent() = default;

    // フォールバック描画色を変更する
    void setFallback(Color color);
    // 有効なテクスチャがあればスプライトを、それ以外は円を描画する
    void draw(const Vector2& position, float radius, Color tint = WHITE) const;

private:
    // フォールバック用の単色
    Color m_fallbackColor{RAYWHITE};
};

} // namespace ks
