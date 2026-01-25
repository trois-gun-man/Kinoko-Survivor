#include "StateMachine.hpp"

namespace ks {

// ステートマシンが操作するエンティティを登録する
void StateMachine::setOwner(Entity* owner) {
    m_owner = owner;
}

// 現在の状態をクリーンアップし、新しい状態へ切り替える
void StateMachine::changeState(IState* next) {
    if (m_current && m_owner) {
        m_current->exit(*m_owner);
    }
    m_current = next;
    if (m_current && m_owner) {
        m_current->enter(*m_owner);
    }
}

// 現在の状態があれば更新を実行する
void StateMachine::update(float dt) {
    if (m_current && m_owner) {
        m_current->update(*m_owner, dt);
    }
}

// move 後などで強制的に状態を差し替える（必要なら enter を再実行）
void StateMachine::overrideState(IState* state, bool callEnter) {
    m_current = state;
    if (callEnter && m_current && m_owner) {
        m_current->enter(*m_owner);
    }
}

} // namespace ks
