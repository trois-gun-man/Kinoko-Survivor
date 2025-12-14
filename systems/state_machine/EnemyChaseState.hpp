#pragma once

#include "IState.hpp"

namespace ks {

// 敵がターゲットへ向かって追跡・攻撃する状態
class EnemyChaseState : public IState {
public:
    // 初期化としてアニメーションをリセットする
    void enter(Entity& owner) override;
    // 移動や攻撃タイミングを更新する
    void update(Entity& owner, float dt) override;
    // 退出時に移動フラグを落とす
    void exit(Entity& owner) override;
};

} // namespace ks
