#pragma once

#include <raylib.h>

namespace ks {

// 2D 座標を保持し、移動計算を担当するコンポーネント
class PositionComponent {
public:
    // 座標を絶対値で設定する
    void set(float x, float y);
    // 現在位置に差分を加算する
    void translate(float dx, float dy);
    // raylib 互換の Vector2 を返す
    [[nodiscard]] Vector2 toVector() const;
    // X 座標のみ取得する
    [[nodiscard]] float x() const;
    // Y 座標のみ取得する
    [[nodiscard]] float y() const;

private:
    // 現在の X 座標
    float m_x = 0.0f;
    // 現在の Y 座標
    float m_y = 0.0f;
};

} // namespace ks
