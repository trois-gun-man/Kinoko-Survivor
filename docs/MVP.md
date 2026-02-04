#1 🎮 ゲーム概要（企画書レベル）
タイトル

Kinoko Survivor
C++ × 2D 横スクロール × ローグライト × ミニプロダクション

コンセプト

「横スクロールアクション × 軽ローグライト」

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

## 3.4 AI（意思決定）

GoF：Strategyパターン

AIStrategy

SimpleChaseAI

## 3.5 Factory（生成）

GoF：Factory Method / Abstract Factory

CharacterFactory（Player/Enemy生成）

## 3.7 UI

GoF：Observer パターン（EventBus）

UIHealthBar

UIExpBar

UILevelText

UIPopup

UIResultScreen

## 3.8 Audio

AudioManager
（EventBusからサウンドをトリガー）

## 3.9 Item System
![test](../assets/play_background.png)