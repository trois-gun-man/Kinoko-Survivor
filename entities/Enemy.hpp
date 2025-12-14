#pragma once

#include <memory>
#include <raylib.h>

#include "Entity.hpp"
#include "../components/HealthComponent.hpp"
#include "../components/PositionComponent.hpp"
#include "../components/RenderComponent.hpp"
#include "../systems/ai/AIStrategy.hpp"
#include "../systems/state_machine/StateMachine.hpp"
#include "../systems/state_machine/EnemyChaseState.hpp"

namespace ks {

class Player;

// AI 戦略とステートマシンで行動する敵エンティティ
class Enemy : public Entity {
public:
    // 共通スプライトを読み込む
    static void AcquireSpriteSheet(const char* path);
    // 参照カウントがゼロになったら解放する
    static void ReleaseSpriteSheet();

    Enemy();
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;

    // ステートマシンを進める
    void update(float dt) override;
    // スプライトまたはフォールバックで描画する
    void render() override;

    // 絶対座標を設定する
    void setPosition(float x, float y);
    // 位置を差分で移動させる（Y は地面に固定）
    void translate(float dx, float dy);
    // 現在位置を返す
    [[nodiscard]] Vector2 getPosition() const;
    // 追跡対象プレイヤーを設定する
    void setTarget(const Player* player);
    // ターゲットの有無を調べる
    [[nodiscard]] bool hasTarget() const;
    // ターゲット位置（いない場合は自身）を返す
    [[nodiscard]] Vector2 getTargetPosition() const;
    // 現在の移動速度を返す
    [[nodiscard]] float getSpeed() const;
    // 任意の AI 戦略をセットする
    void setStrategy(std::unique_ptr<AIStrategy> strategy);
    // 当たり半径を取得
    [[nodiscard]] float radius() const;
    // 接地高さを設定
    void setGround(float y);
    // 横方向の行動範囲を設定
    void setMovementBounds(float minX, float maxX);
    // 1 フレーム限りの攻撃イベントを消費する
    bool consumeAttackEvent();

private:
    // 移動範囲内に X を収める
    void clampToBounds();
    // 向きや移動フラグを更新する
    void updateAnimationState(float prevX);
    // スプライトで描画する
    void drawSprite() const;
    // フォールバック描画を行う
    void drawFallback() const;
    // ムーブ後のポインタ参照を補正する
    void resetStateMachine();

    // 位置情報
    PositionComponent m_position;
    // スプライトまたは円の描画
    RenderComponent m_render;
    // 体力管理
    HealthComponent m_health;
    // 現在の AI 戦略
    std::unique_ptr<AIStrategy> m_strategy;
    // 行動ステートマシン
    StateMachine m_stateMachine;
    // 追跡状態ロジック
    EnemyChaseState m_chaseState;
    // 追跡するプレイヤー
    const Player* m_target = nullptr;
    // 初期値をまとめた定数群
    static constexpr int kDefaultMaxHealth = 25;
    static constexpr float kDefaultSpeed = 140.0f;
    static constexpr float kDefaultRadius = 14.0f;
    static constexpr float kDefaultGroundY = 360.0f;
    static constexpr float kDefaultMinX = 0.0f;
    static constexpr float kDefaultMaxX = 800.0f;
    static constexpr float kDefaultAttackRange = 36.0f;
    static constexpr float kDefaultAttackCooldown = 0.8f;
    // 基本移動速度
    float m_speed = kDefaultSpeed;
    // 当たり半径
    float m_radius = kDefaultRadius;
    // 足場高さ
    float m_groundY = kDefaultGroundY;
    // 行動範囲の左端
    float m_minX = kDefaultMinX;
    // 行動範囲の右端
    float m_maxX = kDefaultMaxX;
    // アニメーション用経過時間
    float m_animTimer = 0.0f;
    // 移動しているか
    bool m_isMoving = false;
    // 左向きフラグ
    bool m_facingLeft = false;
    // 攻撃モーション中か
    bool m_isAttacking = false;
    // 攻撃可能距離
    float m_attackRange = kDefaultAttackRange;
    // 攻撃クールダウン
    float m_attackCooldown = kDefaultAttackCooldown;
    // クールダウン残り
    float m_attackTimer = 0.0f;
    // 攻撃トリガーが発生したか
    bool m_attackTriggered = false;

    // 敵共通スプライト
    static inline Texture2D s_spriteSheet{};
    // 読み込み済みか
    static inline bool s_spriteLoaded = false;
    // 使用しているインスタンス数
    static inline int s_spriteUsers = 0;
    // スプライトの列数
    static constexpr int s_spriteColumns = 4;
    // スプライトの行数
    static constexpr int s_spriteRows = 3;
    // フレーム切り替え間隔
    static constexpr float s_frameDuration = 0.12f;
    friend class EnemyChaseState;
};

} // namespace ks
