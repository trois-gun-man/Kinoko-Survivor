#pragma once

#include <array>
#include <functional>

#include <raylib.h>

#include "GameState.hpp"
#include "StateManager.hpp"

namespace ks {

class ResultState : public GameState {
public:
    ResultState(float survivalTime, std::function<void(StateManager&)> retryAction);

    void Update(StateManager& manager) override;
    void Draw() override;

private:
    float m_survivalTime = 0.0f;
    std::function<void(StateManager&)> m_retryAction;
    int m_selected = 0;
    std::array<const char*, 2> m_options{"RETRY", "EXIT"};

    void handleInput(StateManager& manager);
    void moveSelection(int delta);
    void activateSelection(StateManager& manager);
    void drawPanel() const;
    void drawSurvivalTime(const Rectangle& panel) const;
    void drawOptions(const Rectangle& panel) const;
};

} // namespace ks
