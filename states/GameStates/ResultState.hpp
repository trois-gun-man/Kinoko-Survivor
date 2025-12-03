#pragma once

#include "GameState.hpp"

namespace ks {

class ResultState : public GameState {
public:
    void Update(StateManager& manager) override;
    void Draw() override;
};

} // namespace ks
