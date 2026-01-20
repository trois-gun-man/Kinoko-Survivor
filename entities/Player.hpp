#pragma once

#include <raylib.h>

#include "Entity.hpp"
#include "../components/HealthComponent.hpp"
#include "../components/PositionComponent.hpp"
#include "../components/RenderComponent.hpp"
#include "../systems/state_machine/StateMachine.hpp"
#include "../systems/state_machine/PlayerMoveState.hpp"
#include "../systems/state_machine/PlayerAttackState.hpp"

namespace ks {

// プレイヤー操作に必要な入力処理と描画を担うエンティティ
class Player : public Entity {
public:
    enum class Facing {
        Left,
        Right
    };

    Player();

    // ステートマシンで行動を更新する
    void update(float dt) override;
    // 現在の見た目を描画する
    void render() override;

    // 現在位置を取得する
    [[nodiscard]] Vector2 getPosition() const;
    // 受けたダメージを体力へ反映する
    void applyDamage(int amount);
    // 体力が尽きたか判定する
    [[nodiscard]] bool isDead() const;
    // 当たり判定半径を返す
    [[nodiscard]] Vector2 radius() const;
    // 現在 HP を返す
    [[nodiscard]] int health() const;
    // 最大 HP を返す
    [[nodiscard]] int maxHealth() const;
    // 接地 Y 座標を設定する
    void setGround(float y);
    // 水平方向の移動可能範囲を設定する
    void setMovementBounds(float minX, float maxX);
    // X 座標を直接指定する
    void setPosition(float x);
    // 入力方向と dt を受け取り、内部で移動計算を行う
    void moveHorizontally(float direction, float dt);
    void jump();
    bool isOnGround() const;
    void attack();
    void draw();
    void setFirstAttack();
    bool getFirstAttack();
    StateMachine& getStateMachine(); 
    PlayerAttackState& getAttackState(); 
    PlayerMoveState& getMoveState(); 
    Rectangle getAttackHitBox() const;
    bool isAttacking() const;
    void setAttackHit(bool hit);
    bool getAttackHit() const;


private:
    Facing m_facing = Facing::Right;
    Facing m_attackFacing = Facing::Right;

    // 現在位置を移動範囲へ収める
    void clampToBounds();
    // 位置や移動量
    PositionComponent m_position;
    // スプライト描画情報
    RenderComponent m_render;
    // 体力の管理
    HealthComponent m_health;
    // 行動の切り替えを管理するステートマシン
    StateMachine m_stateMachine;
    // 実際の行動ロジック
    PlayerMoveState   m_moveState;
    PlayerAttackState m_attackState;
    static constexpr float kDefaultSpeed = 220.0f;
    static constexpr Vector2 kDefaultRadius = {25.0f, 36.0f};
    static constexpr float kDefaultGroundY = 400.0f;
    static constexpr float kDefaultMinX = 0.0f;
    static constexpr float kDefaultMaxX = 800.0f;
    static constexpr int kDefaultMaxHealth = 100;
    // 移動速度
    float m_speed = kDefaultSpeed;
    // 描画・当たり判定半径
    Vector2 m_radius = kDefaultRadius;
    // 足場の高さ
    float m_groundY = kDefaultGroundY;

    float m_minX = kDefaultMinX;
    float m_maxX = kDefaultMaxX;
    float velocityY = 0.0f;

    bool m_isAttacking = false;
    float m_attackTimer = 0.0f;
    bool first_attack = true;
    bool m_attackHit = false;
};



} // namespace ks
