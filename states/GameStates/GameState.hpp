#pragma once

class StateManager;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void Update(StateManager& manager) = 0;
    virtual void Draw() = 0;
};
