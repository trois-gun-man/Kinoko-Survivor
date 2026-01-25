#pragma once

#include "IState.hpp"

namespace ks {

// 1 つのエンティティに紐づく状態遷移と実行を管理する
class StateMachine {
public:
    // 動作対象となるエンティティを登録する
    void setOwner(Entity* owner);
    // 現在の状態を終了し、新しい状態へ切り替える
    void changeState(IState* next);
    // 現在の状態を更新する
    void update(float dt);
    // move 後などに強制的に状態ポインタを差し替える
    void overrideState(IState* state, bool callEnter);

private:
    // 振る舞いを提供するエンティティ
    Entity* m_owner = nullptr;
    // 現在アクティブな状態
    IState* m_current = nullptr;
};

} // namespace ks
