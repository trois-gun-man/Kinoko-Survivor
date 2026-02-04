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

弱攻撃

回避

チャージ攻撃（弱 → 強）

コンボ（弱 → 弱 → 強）

パリィ（タイミング可）

見切り（スローモーション）

武器：

ナイフ（近距離）

ダガー（高速）

木の棒（範囲）

弓（遠距離）

魔法（範囲）

銃（貫通）

敵（エネミー）

種類：3種のキノコ（+ エリート + ボス）

出現比率：

雑魚85%

エリート15%（1分ごとに1体確定湧き）

ボス：4分経過でボス部屋（固定ステージ）

敵タイプ：

歩行型（オーガ）

飛行型（ハーピィ）

ボス：

移動なし

ブレス攻撃のみ（敷居を下げるため）

ローグライト要素

敵がランダム出現

宝箱ランダム配置

アイテムバッグからランダムアイテム

永続強化（タロット/アーティファクト/呪い）

NPC（イベント系）

商人：アイテム購入

回復の泉：デバフ解除/回復

女神：ランダム恩恵

地味系女子：ランダム好感度イベント

悪魔の子ども：呪いデバフ

成長（レベルアップ曲線）
必要経験値（例）

Lv1：1
Lv2：5
Lv3：20
Lv4：50
Lv5：100
Lv6：500
Lv7：1500
Lv8：8000
Lv9：20000
Lv10：50000

敵の経験値
exp = 0.345 * t * Rare
t = 経過時間, Rare = 敵レアリティ係数

#3 🏗 ゲームアーキテクチャ（GoF23準拠）

ゲームは以下4レイヤーで構成する。

[ Scene State Layer ] ・・・ ゲーム画面の遷移
[ Domain Layer ] ・・・ キャラの行動・AI
[ System Layer ] ・・・ ステージ・アイテム・UI・Audio
[ Infrastructure Layer ] ・・・ C++/SFML/SDL2 ラッパー

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

RangedAI

## 3.5 Factory（生成）

GoF：Factory Method / Abstract Factory

CharacterFactory（Player/Enemy生成）

ItemFactory

## 3.6 Item（戦闘補助）

GoF：Strategy パターン

ItemBase

BuffStrategy

AttackBuff / SpeedBuff / HealBuff

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

🧩 完全クラス図（Mermaid mmd版）も必要なら作成可能
#4 🧪 テスト計画（プロダクション品質）
単体テスト

StateMachineの遷移テスト

衝突判定テスト

AI Strategy の動作確認

PlayerStats 増減テスト

ItemFactory のランダム性テスト

結合テスト

PlayState → Enemy → LootDropper

HP UI の連動

AudioManager のサウンドトリガー

シナリオテスト

4分経過でボスへ遷移

NPCイベントの発生条件

永続強化適用の正しさ

#5 📅 8週間のロードマップ（プロフェッショナル版）
Week 1：アーキテクチャ確定 + キャラデザイン + プロトタイプ

Entity / StateMachine 骨組み

StartState → PlayState 遷移

キャラ初期モーション

Week 2：Player 基本アクション

移動 / ジャンプ

攻撃（弱/強/コンボ）

回避 / パリィ

Week 3：Enemy 基盤 + AI Strategy

EnemyBase

3種のキノコ雑魚

SimpleChaseAI

Week 4：ステージ & スポーン

スクロールステージ

敵ランダム出現

宝箱ランダム

Week 5：ローグライト要素

ItemFactory

Buff封入

アイテムバッグの抽選

Week 6：UI & 不足アクション

HPバー

レベルアップUI

Result画面

プレイヤーパリィ/見切り演出

Week 7：バランス & エリート/ボス

エリート湧き

ボスブレス攻撃

EXP曲線調整

Week 8：総仕上げ

テスト

チューニング

最終ビルド