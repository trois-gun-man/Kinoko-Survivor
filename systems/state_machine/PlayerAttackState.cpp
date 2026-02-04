
#include "PlayerAttackState.hpp"

#include <raylib.h>
#include <iostream>

#include "../../entities/Player.hpp"

namespace ks {

// 入場時は特に初期化不要
void PlayerAttackState::enter(Entity& owner) {
    (void)owner;
}

// 左右入力を集約し、速度と dt を掛けて位置を更新する
void PlayerAttackState::update(Entity& owner, float dt) {
    auto* player = dynamic_cast<Player*>(&owner);
    if (!player) {
        return;
    }
    if (player->getFirstAttack()){ 
		player->attack(); 
		//player->draw(); 
	}
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) { 
		//std::cout << "Player Attack State aaa!" << std::endl;
		player->attack(); 
		//player->draw(); 
	}

    float direction = 0.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) { direction -= 1.5f; }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { direction += 1.5f; }

    player->moveHorizontally(direction, dt);

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        player->jump();
    }

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && ( IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D) || 
	    IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_RIGHT)) ) {
		player->getStateMachine().changeState(&player->getMoveState());
	}
}

// 退出時も何もしない
void PlayerAttackState::exit(Entity& owner) {
    (void)owner;
}

} // namespace ks
