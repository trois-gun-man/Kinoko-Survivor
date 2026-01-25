#pragma once

namespace ks {

class StateManager;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void Update(StateManager& manager) = 0;
    virtual void Draw() = 0;
};

} // namespace ks
