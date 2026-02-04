#include <raylib.h>
#include "states/GameStates/StateManager.hpp"
#include "states/GameStates/StartState.hpp"

using ks::StateManager;
using ks::StartState;

int main() {
    InitWindow(800, 450, "Kinoko Survivor");
    SetTargetFPS(60);

    StateManager manager;
    manager.ChangeState<StartState>(); 
    InitAudioDevice();
    SetMasterVolume(1.0f);   // ← 明示

    while (!WindowShouldClose()) {
        BeginDrawing();
        manager.Draw();
        manager.Update();
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
