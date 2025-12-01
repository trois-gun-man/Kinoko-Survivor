#pragma once

class StateManager;

class State {
public:
    virtual ~State() = default;

    virtual void Update(StateManager& manager) = 0;
    virtual void Draw() = 0;
};
