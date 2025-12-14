#include "ResultState.hpp"

#include <utility>

namespace ks {

ResultState::ResultState(float survivalTime, std::function<void(StateManager&)> retryAction)
    : m_survivalTime(survivalTime), m_retryAction(std::move(retryAction)) {}

void ResultState::Update(StateManager& manager) {
    handleInput(manager);
}

void ResultState::Draw() {
    ClearBackground(Color{8, 6, 20, 255});
    drawPanel();
}

void ResultState::handleInput(StateManager& manager) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        moveSelection(-1);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        moveSelection(1);
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        activateSelection(manager);
    }
}

void ResultState::moveSelection(int delta) {
    const int optionCount = static_cast<int>(m_options.size());
    m_selected = (m_selected + delta + optionCount) % optionCount;
}

void ResultState::activateSelection(StateManager& manager) {
    if (m_selected == 0) {
        if (m_retryAction) {
            m_retryAction(manager);
        }
    } else {
        CloseWindow();
    }
}

void ResultState::drawPanel() const {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    const Rectangle panel{
        screenW * 0.2f,
        screenH * 0.2f,
        screenW * 0.6f,
        screenH * 0.6f
    };

    DrawRectangleRounded(panel, 0.08f, 8, Color{20, 18, 48, 230});
    DrawRectangleRoundedLines(panel, 0.08f, 8, Color{120, 130, 255, 120});

    const char* title = "RESULT";
    const int titleSize = 48;
    const int titleWidth = MeasureText(title, titleSize);
    const int titleX = static_cast<int>(panel.x + (panel.width - titleWidth) * 0.5f);
    DrawText(title, titleX, static_cast<int>(panel.y) + 30, titleSize, Color{250, 250, 255, 255});

    drawSurvivalTime(panel);
    drawOptions(panel);
}

void ResultState::drawSurvivalTime(const Rectangle& panel) const {
    const int totalSeconds = static_cast<int>(m_survivalTime + 0.5f);
    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;
    const char* timeText = TextFormat("Survival Time: %02d:%02d", minutes, seconds);
    const int fontSize = 32;
    const int textWidth = MeasureText(timeText, fontSize);
    const int x = static_cast<int>(panel.x + (panel.width - textWidth) * 0.5f);
    const int y = static_cast<int>(panel.y + panel.height * 0.45f);
    DrawText(timeText, x, y, fontSize, Color{200, 220, 255, 255});
}

void ResultState::drawOptions(const Rectangle& panel) const {
    const int fontSize = 28;
    const int spacing = 18;
    const int startY = static_cast<int>(panel.y + panel.height * 0.6f);

    for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
        const int textWidth = MeasureText(m_options[i], fontSize);
        const int x = static_cast<int>(panel.x + (panel.width - textWidth) * 0.5f);
        const int y = startY + i * (fontSize + spacing);
        const Color color = (i == m_selected) ? Color{255, 255, 255, 255} : Color{170, 180, 210, 255};
        DrawText(m_options[i], x, y, fontSize, color);

        if (i == m_selected) {
            DrawRectangleLines(x - 30, y - 10, textWidth + 60, fontSize + 20, color);
        }
    }
}

} // namespace ks
