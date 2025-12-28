#1 🎮 ゲーム概要（企画書レベル）
タイトル

Kinoko Survivor
C++ × 2D 横スクロール × ローグライト × ミニプロダクション

コンセプト

「横スクロールアクション × 軽ローグライト」
1ステージで遊べるが、毎回異なる要素でリプレイ性あり。
3人チーム × 8週間で完成できる規模。

#2 🧭 ゲームデザイン（GDD）
プレイヤーアクション
必須：
移動
ジャンプ
攻撃
武器：
身体

敵（エネミー）
種類：1種のキノコ
敵タイプ：
近接タイプのみ
ローグライト要素
敵がランダム出現


#3 🏗 ゲームアーキテクチャ（GoF23準拠）

ゲームは以下4レイヤーで構成する。

[ Scene State Layer ] ・・・ ゲーム画面の遷移
[ Domain Layer ] ・・・ キャラの行動・AI
[ System Layer ] ・・・ ステージ・アイテム・UI・Audio
[ Infrastructure Layer ] ・・・ Raylib

## 3.1 Scene State（画面遷移）

GoF：Stateパターン

GameState（抽象クラス）

StartState

PlayState

ResultState

GameStateManager（Scene専用の StateMachine）

## 3.2 Entity（キャラの抽象基底）

GoF：Bridgeパターン（構造と振る舞いを分離）

Entity

PositionComponent

RenderComponent

HealthComponent

StateMachine（Entity行動用）

## 3.3 Action State（行動ステート）

GoF：Stateパターン

IState

PlayerMoveState

PlayerAttackState

PlayerDamageState

EnemyChaseState

## 3.4 敵行動ロジック

GoF：Stateパターン（EnemyChaseState に直接実装）

- 現状はレーン上を左右にパトロールするだけのシンプル挙動。
- 追跡 AI を導入する場合は Strategy パターンを再検討し、`AIStrategy` を復活させる。

## 3.5 Factory（生成）

GoF：Factory Method / Abstract Factory
EnemySpawner