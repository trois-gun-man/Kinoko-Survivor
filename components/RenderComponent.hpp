#pragma once

#include <raylib.h>

namespace ks {

// スプライト描画と単色フォールバックをまとめる表示用コンポーネント
class RenderComponent {
public:
    RenderComponent() = default;
    ~RenderComponent();

    // 指定パスのテクスチャを読み込み、既存のものがあれば解放する
    void loadSprite(const char* path);
    // フォールバック描画色を変更する
    void setFallback(Color color);
    // 有効なテクスチャがあればスプライトを、それ以外は円を描画する
    void draw(const Vector2& position, Vector2 size, Color tint = WHITE) const;

private:
    // 描画に使用するテクスチャ
    Texture2D m_sprite{};
    // テクスチャを保持しているかどうか
    bool m_hasSprite = false;
    // フォールバック用の単色
    Color m_fallbackColor{RAYWHITE};
};

} // namespace ks
