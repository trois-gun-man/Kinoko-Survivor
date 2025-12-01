#pragma once
#include <raylib.h>
#include "State.hpp"
#include "StateManager.hpp"
#include "GameState.hpp"

class StartState : public State {
public:
    void Update(StateManager& manager) override {
        if (IsKeyPressed(KEY_ENTER)) {
            manager.ChangeState<GameState>();
        }
    }

    void Draw() override {
        ClearBackground(BLACK);
        DrawText("PRESS ENTER TO START", 100, 200, 40, WHITE);
    }
};
