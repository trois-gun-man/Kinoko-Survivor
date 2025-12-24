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
- **役割**: 敵キャラクタ（追跡AI）
- **主要データ**: 共有スプライト、`PositionComponent`, `RenderComponent`, `HealthComponent`, `StateMachine`, `AIStrategy`, 攻撃タイマー
- **主要操作**: `update`, `render`, `setStrategy`, `triggerAttack`
- **依存関係**: `StateMachine`, `EnemyChaseState`, `AIStrategy`
- **備考 / TODO**: エリート/ボス向け追加パラメータ、攻撃通知イベント

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
- **役割**: 敵の移動・攻撃ロジックをフレーム更新
- **主要データ**: 攻撃クールダウン、目標追跡、向き
- **主要操作**: `update` 内で `AIStrategy::decideAction`, 攻撃判定、向き更新
- **依存関係**: `Enemy`, `AIStrategy`
- **備考 / TODO**: レンジ攻撃、特殊アクション対応

---

## 4. AI / ファクトリ / アイテム

### AIStrategy / SimpleChaseAI / RangedAI
- **役割**: 敵挙動アルゴリズムの差し替え
- **主要データ**: 目標距離、移動スピード係数など
- **主要操作**: `decideAction(Enemy&, float dt)`
- **依存関係**: `Enemy`
- **備考 / TODO**: エリート/ボス AI の追加インターフェース整備

### CharacterFactory / EnemySpawner / ItemFactory / TreasureChest
- **役割**: エンティティ・アイテム生成の集中化
- **主要データ**:
  - EnemySpawner: スポーン範囲、波定義、乱数エンジン
  - TreasureChest: 報酬定義
- **主要操作**:
  - EnemySpawner: `update`, `spawnWave`
  - CharacterFactory: `createPlayer`, `createEnemy` (予定)
  - ItemFactory: `createRandomBuff` (予定)
  - TreasureChest: `open`, `giveReward`
- **依存関係**: `Player`, `Enemy`, `BuffStrategy`
- **備考 / TODO**:
  - Factory 実実装、DI 対応、宝箱演出

### BuffStrategy / ItemBase / AttackBuff / SpeedBuff / HealBuff
- **役割**: アイテム効果ポリモーフィズム
- **主要データ**: 効果量、継続時間（予定）
- **主要操作**: `BuffStrategy::apply(Entity&)`
- **依存関係**: `Entity`
- **備考 / TODO**: 具体的な効果ロジックと UI 通知の実装

---

## 5. イベント / UI / オーディオ

### EventBus (`systems/events/EventBus.hpp`)
- **役割**: Observer パターンによるゆるやかな通知
- **主要データ**: `std::unordered_map<int, std::vector<Callback>>`
- **主要操作**: `subscribe(int, Callback)`, `publish(int, void*)`
- **依存関係**: UI, Audio, Gameplay システム
- **備考 / TODO**: イベント ID を列挙化、スレッドセーフ処理、解除 API

### UI クラス群 (`ui/`)
- **役割**: HP/EXP/レベル/ポップアップ/結果画面の表示
- **主要データ**: EventBus 参照、表示用メトリクス、フォント/色設定
- **主要操作**: `render()`, `onEvent(...)`
- **依存関係**: `EventBus`, raylib
- **備考 / TODO**: イベント購読処理、描画ロジック、レイアウト定義

### AudioManager (`audio/AudioManager.hpp`)
- **役割**: ゲームイベントに応じた SE/BGM 再生
- **主要データ**: サウンドハンドル、イベント購読
- **主要操作**: `initialize`, `update`, `onEvent`
- **依存関係**: `EventBus`, raylib Audio API
- **備考 / TODO**: 音量管理、リソースロード/解放、BGM ループ制御

---

## 6. ステージ / 背景

### StageBasic / BackgroundScroll
- **役割**: ステージリソースの読み込みと背景演出
- **主要データ**: 背景テクスチャ、スクロールパラメータ
- **主要操作**: `load`, `update`, `draw`
- **依存関係**: raylib、`PlayState`
- **備考 / TODO**: 実データロード、難易度別レイアウト、パララックス実装

---

## 7. 今後の横断的タスク
- EventBus 実装と UI/Audio 連動により、Health/Attack 等のイベント配線を完了させる
- Player 攻撃・ダメージステート、およびヒットボックス導入で戦闘サイクルを確立
- Factory/Item 実装により成長システムを実体化し、スコア/EXP 表示と連携
- ステージ演出やアート、サウンド追加による臨場感向上
- 各クラスの単体テストフック（依存注入・モック）を整備し、リグレッション防止
