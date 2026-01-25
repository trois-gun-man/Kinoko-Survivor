#pragma once

#include "IState.hpp"

namespace ks {

// 攻撃アニメーションやヒット処理用に拡張予定の状態
class PlayerAttackState : public IState {
public:
    // TODO: 攻撃初期化
    void enter(Entity& owner) override;
    // TODO: 攻撃更新
    void update(Entity& owner, float dt) override;
    // TODO: 攻撃終了処理
    void exit(Entity& owner) override;
};

} // namespace ks
