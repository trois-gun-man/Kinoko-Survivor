#include "PlayerMoveState.hpp"

#include <raylib.h>

#include "../../entities/Player.hpp"

namespace ks {

// 入場時は特に初期化不要
void PlayerMoveState::enter(Entity& owner) {
    (void)owner;
}

// 左右入力を集約し、速度と dt を掛けて位置を更新する
void PlayerMoveState::update(Entity& owner, float dt) {
    auto* player = dynamic_cast<Player*>(&owner);
    if (!player) {
        return;
    }

    float direction = 0.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) { direction -= 1.0f; }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { direction += 1.0f; }

    player->moveHorizontally(direction, dt);
    player->setFirstAttack();

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        player->jump();
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) { 
		player->getStateMachine().changeState(&player->getAttackState());
	}
}

// 退出時も何もしない
void PlayerMoveState::exit(Entity& owner) {
    (void)owner;
}

} // namespace ks
