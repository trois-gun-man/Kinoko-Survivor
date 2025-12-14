#pragma once

namespace ks {

class Entity;

// すべての状態が実装する共通インターフェース
class IState {
public:
    virtual ~IState() = default;
    // 状態に入った瞬間に呼ばれる初期化処理
    virtual void enter(Entity& owner) = 0;
    // 毎フレームの更新処理
    virtual void update(Entity& owner, float dt) = 0;
    // 状態から抜けるときの後処理
    virtual void exit(Entity& owner) = 0;
};

} // namespace ks
