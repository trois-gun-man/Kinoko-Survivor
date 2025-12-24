# レイヤー別フォルダ / クラス構成（最新版）

`docs/mermaid.md` のクラス図と現在のソースを突き合わせ、実装済みレイヤーとスタブ状態の要素を整理した。フォルダ構成全体を俯瞰できるようディレクトリツリー、レイヤー別の役割表、詳細説明、現状サマリをこの 1 ファイルに集約している。

```
d:\gameProduct
├─ main.cpp                        // ゲームループのエントリ
├─ states/GameStates/              // Start / Play / Result + StateManager
├─ entities/                       // Entity 抽象 / Player / Enemy
├─ components/                     // Position / Render / Health
├─ systems/
│   ├─ state_machine/              // IState / StateMachine / PlayerMove / EnemyChase
│   ├─ ai/                         // AIStrategy / SimpleChaseAI / RangedAI
│   ├─ factories/                  // EnemySpawner 実装 / 他はスタブ
│   ├─ items/                      // BuffStrategy / Attack/Speed/Heal (スタブ)
│   └─ events/                     // EventBus スタブ
├─ stages/                         // StageBasic / BackgroundScroll スタブ
├─ ui/                             // UIHealthBar など UI スタブ
├─ audio/                          // AudioManager スタブ
├─ assets/                         // 画像・サウンド
└─ docs/                           // 設計資料（本ファイル含む）
```

| レイヤー | 主担当フォルダ | 代表クラス / インターフェース | 実装状況 | 備考 |
| --- | --- | --- | --- | --- |
| アプリケーション (State) | `main.cpp`, `states/GameStates` | `StateManager`, `GameState`, `StartState`, `PlayState`, `ResultState` | 実装済み | `main.cpp` → `StateManager.ChangeState<StartState>()` で起動。`PlayState` から `ResultState` へ遷移済み。 |
| エンティティ基盤 (Entity) | `entities/`, `components/` | `Entity`, `Player`, `Enemy`, `PositionComponent`, `RenderComponent`, `HealthComponent` | 実装済み | `Player`/`Enemy` が Component を集約。`Enemy` は共有スプライトや攻撃トリガあり。 |
| 行動制御 (StateMachine) | `systems/state_machine` | `StateMachine`, `IState`, `PlayerMoveState`, `EnemyChaseState`, `PlayerAttackState(スタブ)` | コア実装済み | `PlayerMoveState`/`EnemyChaseState` 稼働。攻撃・ダメージ系ステートは TODO。 |
| AI ストラテジ | `systems/ai` | `AIStrategy`, `SimpleChaseAI`, `RangedAI` | 実装済み | `Enemy` が `std::unique_ptr<AIStrategy>` を保持し `decideAction()` を委譲。 |
| ファクトリ / スポナー | `systems/factories` | `EnemySpawner`, `CharacterFactory(スタブ)`, `ItemFactory(スタブ)`, `TreasureChest(スタブ)` | スポナーのみ実装 | `EnemySpawner` が波生成やレーン設定を担当。その他は API 宣言のみ。 |
| アイテム効果 | `systems/items` | `ItemBase`, `BuffStrategy`, `AttackBuff`, `SpeedBuff`, `HealBuff` | スタブ | インターフェースのみで処理未定義。 |
| UI & イベント | `ui/`, `systems/events` | `UIHealthBar`, `UIExpBar`, `UILevelText`, `UIPopup`, `UIResultScreen`, `EventBus` | 未実装 | すべてヘッダのみ。EventBus も subscribe/publish 宣言だけ。 |
| オーディオ | `audio/` | `AudioManager` | 未実装 | EventBus 連携前提の骨組みのみ。 |
| ステージ / 表現 | `stages/` | `StageBasic`, `BackgroundScroll` | 未実装 | API シグネチャのみ。 |

## レイヤー詳細

### 1. アプリケーション / ステートレイヤー
- `main.cpp` は raylib ウィンドウを初期化し、ループ中に `StateManager.Update()` → `Draw()` を毎フレーム呼び出す。
- `StateManager` は `std::unique_ptr<GameState>` を保持し、テンプレート `ChangeState` で遷移。
- `StartState` ではタイトル画像アニメとメニュー入力を処理、`PlayState` ではプレイ中ロジックを更新、`ResultState` でリザルトメニューを描画し `RETRY/EXIT` を扱う。

### 2. エンティティ / コンポーネントレイヤー
- `Entity` は `update(float)` / `render()` を純粋仮想で定義。
- `Player` は `Position/Render/Health` と `StateMachine` を保持し、`moveHorizontally()` や `applyDamage()` などの API を提供。
- `Enemy` はスプライト共有管理、`AIStrategy` 差し替え、攻撃レンジ／クールダウン管理、`EnemyChaseState` 連携を実装。
- `PositionComponent`/`RenderComponent`/`HealthComponent` は責務分割された軽量コンポーネントで、`Player`/`Enemy` から直接利用されている。

### 3. 行動制御 (StateMachine) レイヤー
- `StateMachine` がエンティティ固有の `IState` を保持し、`changeState()` で `exit/enter` を呼び分ける。
- `PlayerMoveState` は入力から方向を読み `Player::moveHorizontally()` に伝達。
- `EnemyChaseState` は `AIStrategy` の結果を監視し、移動範囲の clamp、アニメーション状態更新、攻撃イベントを `Enemy` に書き込む。
- `PlayerAttackState` / `PlayerDamageState` は TODO のままなので、近々の実装対象として残っている。

### 4. ファクトリ / AI / アイテムレイヤー
- `EnemySpawner` が唯一完成しており、プレイヤー参照・レーン幅・波パラメータを元に `std::vector<Enemy>` に敵を追加する。
- `CharacterFactory`/`ItemFactory`/`TreasureChest` はメソッド宣言のみ。依存計画はあるがコードは未着手。
- `AIStrategy` 派生 (`SimpleChaseAI`, `RangedAI`) はそれぞれ接近・間合い維持ロジックを備え、`Enemy` で `setStrategy()` によって注入できるようになっている。
- `items` 配下の Buff クラスは `apply(Entity&)` をオーバーライドする予定のプレースホルダ。

### 5. UI / オーディオ / イベント
- `ui` フォルダの各クラスは `EventBus&` を受け取って HUD を更新する想定だが、描画ロジックや購読処理はまだ無い。
- `EventBus` も subscribe/publish のシグネチャだけで、内部実装は未決定。
- `AudioManager` は `update(float)` とコンストラクタのみ宣言されており、実際のサウンド制御はこれから。

### 6. ステージ / ビジュアル
- `StageBasic` と `BackgroundScroll` は今後の演出レイヤー用スタブ。現在は `PlayState` が背景描画を直接担っているため、将来的にこれらへ委譲する。

## 現状フォルダ別サマリ

| フォルダ | 主要ファイル | 状態 |
| --- | --- | --- |
| `main.cpp` | メインループ | 完成。`SetTargetFPS(60)` と `StateManager` 中心のループのみ。 |
| `components/` | `HealthComponent`, `PositionComponent`, `RenderComponent` | 全て `.cpp` 含めて稼働。テクスチャ管理や clamping まで実装済み。 |
| `entities/` | `Entity`, `Player`, `Enemy` | 完成。`Enemy` はムーブ/コピー禁止とムーブ後の `resetStateMachine()` など詳細実装あり。 |
| `states/GameStates/` | `GameState`, `StateManager`, `StartState`, `PlayState`, `ResultState` | 3 ステート構成が稼働。リザルトで `RETRY` → `PlayState` へのラムダ遷移も実装済み。 |
| `systems/state_machine/` | `StateMachine`, `IState`, `PlayerMoveState`, `EnemyChaseState`, `PlayerAttackState`, `PlayerDamageState` | 実働ステート 2 種、スタブ 2 種。StateMachine は `overrideState()` まで用意。 |
| `systems/ai/` | `AIStrategy`, `SimpleChaseAI`, `RangedAI` | 完成。`SimpleChaseAI` は距離 1px で停止、`RangedAI` は 180±20 を維持。 |
| `systems/factories/` | `EnemySpawner`, `CharacterFactory`, `ItemFactory`, `TreasureChest` | `EnemySpawner` 以外未着手。スポーン波設定やプレイヤー追従は実装済み。 |
| `systems/items/` | `ItemBase`, `BuffStrategy`, `Attack/Speed/HealBuff` | すべて宣言のみ。 |
| `systems/events/` | `EventBus` | subscribe/publish の空実装。 |
| `ui/` | `UIHealthBar`, `UIExpBar`, `UILevelText`, `UIPopup`, `UIResultScreen` | コンストラクタ・描画メソッド宣言だけ。 |
| `audio/` | `AudioManager` | EventBus 参照を受け取るだけの骨組み。 |
| `stages/` | `StageBasic`, `BackgroundScroll` | いずれも空のシグネチャ。 |

## 直近の優先タスク案

1. `systems/events/EventBus` の実装 → UI / Audio / Buff へイベント通知経路を確保。
2. `ui` クラス群の実装 → Player HP などを HUD へ分離し、`PlayState::drawUI()` の責務を削減。
3. `systems/items` + `ItemFactory` → Buff 適用処理を具体化し、敵や宝箱との連携を整備。
4. `PlayerAttackState` / `PlayerDamageState` の実装 → `StateMachine` を活かした挙動差し替えを実現。
5. `AudioManager` と効果音再生 → EventBus 経由で攻撃/被弾イベントに追従。

このドキュメントは今後もクラス追加や責務変更があれば更新する想定。差分が発生した場合は `docs/mermaid.md` と合わせて改訂し、WBS と同期させること。

