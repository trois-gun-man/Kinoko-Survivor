#pragma once

namespace ks {

// 体力値の加減算と死活判定だけを担う軽量コンポーネント
class HealthComponent {
public:
    // 最大値を設定し同時に現在値も満タンへリセットする
    void setMax(int value);
    // 減算の結果が負にならないようにしながらダメージを適用する
    void applyDamage(int value);
    // 現在値がゼロ以下かどうかを返し、死亡扱いか判定する
    [[nodiscard]] bool isDead() const;
    // 現在の体力値を取得する
    [[nodiscard]] int current() const;
    // 最大体力値を取得する
    [[nodiscard]] int max() const;

private:
    // 現在の体力値
    int m_current = 0;
    // 上限となる体力値
    int m_max = 0;
};

} // namespace ks
