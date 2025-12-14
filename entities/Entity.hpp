#pragma once

namespace ks {

// ゲーム内の全オブジェクトが共有する最小限のインターフェース
class Entity {
public:
    virtual ~Entity() = default;
    // 経過時間を受け取り、状態更新を行う
    virtual void update(float dt) = 0; // TODO: 振る舞いコンポーネント注入で差し替える
    // 画面へ描画する
    virtual void render() = 0;
};

} // namespace ks
