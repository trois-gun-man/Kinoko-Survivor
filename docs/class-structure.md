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
│   └─ factories/                  // EnemySpawner 実装 / 他はスタブ
├─ assets/                         // 画像・サウンド
└─ docs/                           // 設計資料（本ファイル含む）
```

| レイヤー | 主担当フォルダ | 代表クラス / インターフェース | 実装状況 | 備考 |
| --- | --- | --- | --- | --- |
| アプリケーション (State) | `main.cpp`, `states/GameStates` | `StateManager`, `GameState`, `StartState`, `PlayState`, `ResultState` | 実装済み | `main.cpp` → `StateManager.ChangeState<StartState>()` で起動。`PlayState` から `ResultState` へ遷移済み。 |
| エンティティ基盤 (Entity) | `entities/`, `components/` | `Entity`, `Player`, `Enemy`, `PositionComponent`, `RenderComponent`, `HealthComponent` | 実装済み | `Player`/`Enemy` が Component を集約。`Enemy` は共有スプライトや攻撃トリガあり。 |
| 行動制御 (StateMachine) | `systems/state_machine` | `StateMachine`, `IState`, `PlayerMoveState`, `EnemyChaseState`, `PlayerAttackState(スタブ)` | コア実装済み | `PlayerMoveState`/`EnemyChaseState` 稼働。攻撃・ダメージ系ステートは TODO。 |
| ファクトリ / スポナー | `systems/factories` | `EnemySpawner`, `CharacterFactory(スタブ)`| スポナーのみ実装 | `EnemySpawner` が波生成やレーン設定を担当。その他は API 宣言のみ。 |

## レイヤー詳細

### 1. アプリケーション / ステートレイヤー
- `main.cpp` は raylib ウィンドウを初期化し、ループ中に `StateManager.Update()` → `Draw()` を毎フレーム呼び出す。
- `StateManager` は `std::unique_ptr<GameState>` を保持し、テンプレート `ChangeState` で遷移。
- `StartState` ではタイトル画像アニメとメニュー入力を処理、`PlayState` ではプレイ中ロジックを更新、`ResultState` でリザルトメニューを描画し `RETRY/EXIT` を扱う。

### 2. エンティティ / コンポーネントレイヤー
- `Entity` は `update(float)` / `render()` を純粋仮想で定義。
- `Player` は `Position/Render/Health` と `StateMachine` を保持し、`moveHorizontally()` や `applyDamage()` などの API を提供。
- `Enemy` はスプライト共有管理、レーン内パトロール、攻撃レンジ／クールダウン管理、`EnemyChaseState` 連携を実装。
- `PositionComponent`/`RenderComponent`/`HealthComponent` は責務分割された軽量コンポーネントで、`Player`/`Enemy` から直接利用されている。

### 3. 行動制御 (StateMachine) レイヤー
- `StateMachine` がエンティティ固有の `IState` を保持し、`changeState()` で `exit/enter` を呼び分ける。
- `PlayerMoveState` は入力から方向を読み `Player::moveHorizontally()` に伝達。
- `EnemyChaseState` は一定速度の左右移動と折り返し、アニメーション状態更新、攻撃クールダウン管理を担当。
- `PlayerAttackState` / `PlayerDamageState` は TODO のままなので、近々の実装対象として残っている。

### 4. ファクトリ / アイテムレイヤー
- `EnemySpawner` が唯一完成しており、レーン幅をセットしたあと一定時間後に 1 体だけ生成する仕組みに簡略化されている。


## 現状フォルダ別サマリ

| フォルダ | 主要ファイル | 状態 |
| --- | --- | --- |
| `main.cpp` | メインループ | 完成。`SetTargetFPS(60)` と `StateManager` 中心のループのみ。 |
| `components/` | `HealthComponent`, `PositionComponent`, `RenderComponent` | 全て `.cpp` 含めて稼働。テクスチャ管理や clamping まで実装済み。 |
| `entities/` | `Entity`, `Player`, `Enemy` | 完成。`Enemy` はムーブ/コピー禁止とムーブ後の `resetStateMachine()` など詳細実装あり。 |
| `states/GameStates/` | `GameState`, `StateManager`, `StartState`, `PlayState`, `ResultState` | 3 ステート構成が稼働。リザルトで `RETRY` → `PlayState` へのラムダ遷移も実装済み。 |
| `systems/state_machine/` | `StateMachine`, `IState`, `PlayerMoveState`, `EnemyChaseState`, `PlayerAttackState`, `PlayerDamageState` | 実働ステート 2 種、スタブ 2 種。StateMachine は `overrideState()` まで用意。 |
| `systems/factories/` | `EnemySpawner` | スポナーは単体スポーンのみ実装。 |
|
## 直近の優先タスク案

1. `PlayerAttackState` / `PlayerDamageState` の実装 → `StateMachine` を活かした挙動差し替えを実現。

このドキュメントは今後もクラス追加や責務変更があれば更新する想定。差分が発生した場合は `docs/mermaid.md` と合わせて改訂し、WBS と同期させること。

