#include <raylib.h>
#include "state/StateManager.hpp"
#include "state/StartState.hpp"

int main() {
    InitWindow(800, 450, "State Pattern Example");
    SetTargetFPS(60);

    StateManager manager;
    manager.ChangeState<StartState>();  // 最初はスタート画面

    while (!WindowShouldClose()) {
        BeginDrawing();
        manager.Update();
        manager.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
