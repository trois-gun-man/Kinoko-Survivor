#pragma once
#include <raylib.h>
#include "GameState.hpp"

namespace ks {

class PlayState : public GameState {
public:
    void Update(StateManager& manager) override {
        // ゲーム中の処理（ESCで終了）
        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
        }
    }

    void Draw() override {
        ClearBackground(DARKBLUE);
        DrawText("GAME START!", 120, 200, 40, WHITE);
    }
};

} // namespace ks
