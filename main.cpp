#include <raylib.h>
#include "states/GameStates/StateManager.hpp"
#include "states/GameStates/StartState.hpp"

int main() {
    InitWindow(800, 450, "Kinoko Survivor");
    SetTargetFPS(60);

    StateManager manager;
    manager.ChangeState<StartState>(); 

    while (!WindowShouldClose()) {
        BeginDrawing();
        manager.Update();
        manager.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
