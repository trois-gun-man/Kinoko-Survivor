# Kinoko Survivor クラス基本設計書

本書はプロジェクト内の主要クラスについて、役割・主要責務・インターフェース・依存関係・今後の拡張ポイントを整理し、設計レビューおよび実装計画に共有することを目的とする。

## 記法
- **役割**: クラスが担う機能的責務
- **主要データ**: コアとなるメンバ変数／状態
- **主要操作**: 外部公開 API や重要な内部処理
- **依存関係**: 参照/所有する他クラスやシステム
- **備考 / TODO**: 改善点や今後の実装タスク

---

## 1. アプリケーション / ステート層

### main.cpp
- **役割**: raylib 初期化、メインループ、`StateManager` への委譲
- **主要データ**: ウィンドウ設定、`StateManager` インスタンス
- **主要操作**: `InitWindow`, `SetTargetFPS`, ループ内 `stateManager.Update/Draw`
- **依存関係**: raylib API, `StateManager` と `GameState` 実装
- **備考 / TODO**: ループ終了条件にゲームオーバーシグナルを追加予定

### StateManager (`states/GameStates/StateManager.hpp`)
- **役割**: 現在の `GameState` を保持し、遷移・更新・描画を集中管理
- **主要データ**: `std::unique_ptr<GameState>` currentState
- **主要操作**: `ChangeState<T>()`, `Update()`, `Draw()`
- **依存関係**: `GameState` 抽象、各具体ステート（Start/Play/Result）
- **備考 / TODO**: フェード演出用の遷移フックを将来的に挿入

### GameState (`states/GameStates/GameState.hpp`)
- **役割**: ステート共通インターフェース（`Update(StateManager&)`, `Draw()`）
- **主要データ**: なし（純粋仮想）
- **主要操作**: `Update`, `Draw`, 任意で `OnEnter/OnExit` を実装
- **依存関係**: `StateManager`
- **備考 / TODO**: `OnEnter/OnExit` を正式にインターフェースへ昇格検討

### StartState / PlayState / ResultState
- **役割**: それぞれタイトル、ゲームプレイ、リザルト表示を担当
- **主要データ**:
  - Start: 背景・タイトルテクスチャ、カーソル位置
  - Play: `Player`, `std::vector<Enemy>`, 背景テクスチャ、`EnemySpawner`, 経過時間
  - Result: サバイバル時間、メニュー選択、リトライ用ラムダ
- **主要操作**: `Update` で入力処理と遷移制御、`Draw` で各画面描画
- **依存関係**: `StateManager`, `Player`, `Enemy`, `EnemySpawner`, raylib
- **備考 / TODO**:
  - Play: スコア・経験値 UI, EventBus 連携, ポーズ機能を追加予定
  - Result: 戦績詳細やランキング表示に拡張余地

---

## 2. エンティティ / コンポーネント層

### Entity (`entities/Entity.hpp`)
- **役割**: すべてのゲーム内存在の共通抽象
- **主要データ**: なし（純粋仮想）
- **主要操作**: `update(float dt)`, `render()`
- **依存関係**: なし
- **備考 / TODO**: コンポーネント取得インターフェースを追加検討

### Player (`entities/Player.*`)
- **役割**: プレイヤーキャラクタ本体
- **主要データ**: `PositionComponent`, `RenderComponent`, `HealthComponent`, `StateMachine`, 水平速度, 当たり判定幅, 移動制限領域
- **主要操作**: `update`, `render`, `moveHorizontally`, `takeDamage`, `getHealthInfo`
- **依存関係**: 各コンポーネント、`StateMachine`, `PlayerMoveState`, raylib 入力
- **備考 / TODO**: 攻撃/ダメージ/ジャンプ状態追加、EventBus への HP 通知

### Enemy (`entities/Enemy.*`)
- **役割**: 敵キャラクタ（レーン上を左右にパトロールしてプレイヤーを圧迫）
- **主要データ**: 共有スプライト、`PositionComponent`, `RenderComponent`, `HealthComponent`, `StateMachine`, パトロール方向、移動制限、攻撃クールダウン値
- **主要操作**: `update`, `render`, `setMovementBounds`, `setGround`, `consumeAttackEvent`
- **依存関係**: `StateMachine`, `EnemyChaseState`
- **備考 / TODO**: 行動ロジックはステート側に直書き。将来エリート向けにスピードや攻撃レンジを差し替える計画。

### PositionComponent / RenderComponent / HealthComponent
- **役割**: 位置、描画、体力を個別管理し、Entity へ注入
- **主要データ**:
  - Position: `Vector2 position`
  - Render: `Texture2D sprite`, `Color fallbackColor`
  - Health: `int maxHealth`, `int currentHealth`
- **主要操作**:
  - Position: `set`, `translate`, `toVector`
  - Render: `loadTexture`, `draw`, `unload`
  - Health: `applyDamage`, `heal`, `isDead`
- **依存関係**: raylib (`Vector2`, `Texture2D`)
- **備考 / TODO**: RenderComponent にアニメーション再生、HealthComponent にイベント発火追加

---

## 3. 行動制御 (StateMachine)

### IState / StateMachine
- **役割**: エンティティ固有の状態遷移と更新フレームを抽象化
- **主要データ**: `StateMachine` は `currentState`, `owner` (Entity 派生)
- **主要操作**:
  - IState: `enter(Entity*)`, `update(Entity*, float)`, `exit(Entity*)`
  - StateMachine: `changeState`, `update`, `setOwner`, `overrideState`
- **依存関係**: `Entity` 派生クラス
- **備考 / TODO**: 状態ヒストリ、ガード条件、ステート共有の仕組みを検討

### PlayerMoveState / PlayerAttackState / PlayerDamageState
- **役割**: プレイヤー挙動の細分化
- **主要データ**: なし（ステート内部）
- **主要操作**:
  - Move: 入力読み取り→`Player::moveHorizontally`
  - Attack: TODO（攻撃入力受付、アニメーション制御）
  - Damage: TODO（無敵時間、ノックバック処理）
- **依存関係**: `Player`
- **備考 / TODO**: Attack/Damage の実装、遷移条件の定義、StateMachine 経由の通知

### EnemyChaseState
- **役割**: 敵の左右パトロールと簡易攻撃クールダウン管理
- **主要データ**: パトロール方向、移動スピード、攻撃クールダウン
- **主要操作**: `update` 内で一定速度で移動し、レーン端で折り返しつつアニメーション状態を更新
- **依存関係**: `Enemy`
- **備考 / TODO**: 追跡 AI を再導入する場合はここに専用サブステートや行動テーブルを噛ませる。

---

## 4.  ファクトリ / アイテム

###  EnemySpawner
- **役割**: エンティティ・アイテム生成の集中化
- **主要データ**:
  - EnemySpawner: スポーン範囲、レーン設定、単一出現フラグ
- **主要操作**:
  - EnemySpawner: `setLane`, `update`（初回 1 体のみ生成）
- **依存関係**: `Enemy`

---
