#pragma once

#include "IState.hpp"

namespace ks {

// 被弾リアクションや無敵時間を扱う予定の状態
class PlayerDamageState : public IState {
public:
    // TODO: ダメージ直後の処理
    void enter(Entity& owner) override;
    // TODO: ダメージ中の更新
    void update(Entity& owner, float dt) override;
    // TODO: 無敵終了などの後処理
    void exit(Entity& owner) override;
};

} // namespace ks
