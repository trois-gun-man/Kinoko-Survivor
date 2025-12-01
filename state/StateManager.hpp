#pragma once
#include <memory>
#include "State.hpp"

class StateManager {
private:
    std::unique_ptr<State> currentState;

public:
    template <typename T, typename... Args>
    void ChangeState(Args&&... args) {
        currentState = std::make_unique<T>(std::forward<Args>(args)...);
    }

    void Update() {
        if (currentState) currentState->Update(*this);
    }

    void Draw() {
        if (currentState) currentState->Draw();
    }
};
