# レイヤー別フォルダ / クラス構成

プロジェクト内のフォルダ構成をレイヤー単位で整理し、各フォルダに紐づく主要クラスを下表とセクションでまとめる。`docs/mermaid.md` のクラス図および `docs/WBS.csv` の作業項目を反映した計画構成であり、今後の実装指針として利用できる。

```
d:\gameProduct
├─ main.cpp                       // エントリポイント / ゲームループ
├─ states/
│   └─ GameStates/
│        ├─ GameState.hpp        // 抽象ゲームステート
│        ├─ StateManager.hpp     // ステート管理
│        ├─ StartState.hpp       // タイトル / 開始ステート
│        └─ PlayState.hpp        // プレイ中ステート（ResultStateは今後追加）
├─ assets/                       // スプライト・サウンドなどのリソース
├─ docs/                         // 設計資料（本ファイル含む）
├─ entities/                     // ★予定：Entity / Component 実装
├─ components/                   // ★予定：Position, Render, Health など
├─ systems/
│   ├─ state_machine/            // ★予定：StateMachine + IState 実装
│   ├─ factories/                // ★予定：CharacterFactory, ItemFactory
│   ├─ ai/                       // ★予定：AIStrategy, SimpleChaseAI 等
│   └─ items/                    // ★予定：BuffStrategy, AttackBuff 等
├─ ui/                           // ★予定：UIHealthBar, UIResultScreen 等
└─ audio/                        // ★予定：AudioManager
```

表中の「予定」フォルダは WBS の優先タスクとして今後作成する想定のもの。

| レイヤー | 主担当フォルダ | 代表クラス / インターフェース | 役割 | 備考 |
| --- | --- | --- | --- | --- |
| アプリケーション (State) | `main.cpp`, `states/GameStates` | `StateManager`, `GameState`, `StartState`, `PlayState`, `ResultState(予定)` | ゲーム全体のライフサイクル制御、ゲームループ、ステート遷移 | `main.cpp` から `StateManager` を初期化し、入力/更新/描画を委譲 |
| エンティティ基盤 (Entity) | `entities/` | `Entity`, `Player`, `Enemy`, `ItemBase` | Entity 抽象と派生キャラクタの共通処理 | Bridge パターン: Entity が Component を保持 |
| コンポーネント (Component) | `components/` | `PositionComponent`, `RenderComponent`, `HealthComponent` | データ/機能を細分化し、Entity が組み合わせて利用 | 依存を減らしテスト容易性を確保 |
| 行動制御 (StateMachine) | `systems/state_machine` | `StateMachine`, `IState`, `PlayerMoveState`, `EnemyChaseState` | Actor 行動の状態遷移と更新ロジック | IState 実装でアクションを差し替え |
| ファクトリ (Factory) | `systems/factories` | `CharacterFactory`, `ItemFactory`, `EnemySpawner`, `TreasureChest` | 生成手順の共通化・依存注入 | プレイヤー/敵/アイテム生成を一元管理 |
| AI ストラテジ (Strategy) | `systems/ai` | `AIStrategy`, `SimpleChaseAI`, `RangedAI` | 敵 AI 方針の差し替え | `Enemy` が保持する戦略オブジェクト |
| アイテム効果 (Item Strategy) | `systems/items` | `BuffStrategy`, `AttackBuff`, `SpeedBuff`, `HealBuff` | 装備/アイテム効果の差し替え | ItemFactory で戦略を組み合わせる |
| イベント & UI (Observer) | `ui/` | `UIHealthBar`, `UIExpBar`, `UIResultScreen`, `EventBus` | HUD/結果画面の更新、イベント購読 | EventBus で HP 変更等を通知 |
| オーディオ (Observer) | `audio/` | `AudioManager` | 効果音/BGM の購読と再生 | EventBus でゲームイベントに追従 |

## レイヤー詳細

### 1. アプリケーション / ステートレイヤー
- `main.cpp` はシングルエントリ。`StateManager` を生成し、現在の `GameState` に更新/描画を委譲。
- `GameState` 抽象は `enter/update/render/exit` などの共通インターフェースを定義。
- `StartState` → `PlayState` → `ResultState` の遷移を `StateManager` が扱い、`states/GameStates` 配下に配置。

### 2. エンティティ / コンポーネントレイヤー
- `entities/` 直下に `Entity` 抽象、`Player`, `Enemy` など派生クラス。
- `components/` フォルダに Position/Render/Health などの小さな責務を持つコンポーネントを配置し、Entity が集約して Bridge パターンを構成。

### 3. 行動制御 (StateMachine) レイヤー
- `systems/state_machine` に `StateMachine` 実装と `IState` インターフェース、および `PlayerMoveState`, `PlayerAttackState`, `EnemyChaseState` 等の具体ステートを格納。
- `StateMachine` はエンティティ固有の `IState` をハンドリングし、`decideAction` などのトリガを実行。

### 4. ファクトリ / AI / アイテムレイヤー
- `systems/factories` で `CharacterFactory`, `ItemFactory`, `EnemySpawner`, `TreasureChest` など生成ロジックを集中管理。
- `systems/ai` では `AIStrategy` 基底と `SimpleChaseAI`, `RangedAI` を実装し、`Enemy` へ依存注入。
- `systems/items` に `BuffStrategy` 派生 (`AttackBuff`, `SpeedBuff`, `HealBuff`) を配置。`ItemFactory` から組み合わせる。

### 5. UI / オーディオレイヤー
- `ui/` 配下で `UIHealthBar`, `UIExpBar`, `UILevelText`, `UIPopup`, `UIResultScreen` など HUD/結果画面を EventBus 経由でイベント購読。
- `audio/` の `AudioManager` も EventBus を購読し、攻撃/被弾/結果イベントに応じて SE/BGM を制御。

### 6. インフラ / 共有サービス
- `systems/` 直下に `EventBus` を配置し、UI/Audio/Gameplay システム間の疎結合な通知を提供。
- 共通ユーティリティは後日 `common/` フォルダを追加し、ログ、設定読み込み、タイマなどを集約する想定。

## 今後の追加タスク抜粋 (WBS 連動)
- `entities/` と `components/` のフォルダ新設および抽象クラス実装（WBS: Create Entity Abstract, Create Component Interfaces）。
- `systems/state_machine` で `IState` と `StateMachine` を実装後、プレイヤー/敵ステートを順次追加。
- `systems/factories` で `CharacterFactory` → `ItemFactory` → `EnemySpawner` の順に実装し、依存フォルダを作成。
- UI (`ui/`) と Audio (`audio/`) は EventBus 実装後に着手し、Observer パターンで接続。

この構成をベースにフォルダ作成とクラス配置を行えば、レイヤー境界が明確になり、責務の分離と拡張の容易性を確保できる。
