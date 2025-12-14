#pragma once

#include "IState.hpp"

namespace ks {

class Player;

// プレイヤーの左右移動のみを扱う状態
class PlayerMoveState : public IState {
public:
    // 入場時の特別処理は無し
    void enter(Entity& owner) override;
    // 入力に応じて左右へ移動させる
    void update(Entity& owner, float dt) override;
    // 退出時の処理は無し
    void exit(Entity& owner) override;
};

} // namespace ks
