#pragma once
#include <array>
#include <cmath>
#include <raylib.h>
#include "GameState.hpp"
#include "StateManager.hpp"
#include "PlayState.hpp"

namespace ks {

class StartState : public GameState {
public:
    StartState() {
        if (FileExists(backgroundPath)) {
            background = LoadTexture(backgroundPath);
        }

        if (FileExists(titlePath)) {
            titleTexture = LoadTexture(titlePath);
        }
    }

    ~StartState() override {
        if (background.id != 0) {
            UnloadTexture(background);
        }

        if (titleTexture.id != 0) {
            UnloadTexture(titleTexture);
        }
    }

    void Update(StateManager& manager) override {
      //  titleAnimTime += GetFrameTime() * titleAnimationSpeed;
        HandleInput(manager);
    }

    void Draw() override {
        DrawBackground();
        DrawTitle();
        DrawMenu();
    }

private:
    using MenuOptions = std::array<const char*, 2>;

    MenuOptions options{"GAME START", "EXIT GAME"};
    int selectedIndex = 0;
    static constexpr const char* backgroundPath = "assets/start_background-v2.png";
    static constexpr const char* titlePath = "assets/start_background-v2-title.png";
   // static constexpr float titleAnimationSpeed = 0.8f;
    static constexpr float titleYOffsetRange = 20.0f;
    static constexpr float titleMaxWidthRatio = 0.505f;
    Texture2D background{};
    Texture2D titleTexture{};
  //  float titleAnimTime = 0.0f;

    void HandleInput(StateManager& manager) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            MoveSelection(-1);
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            MoveSelection(1);
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            ActivateSelection(manager);
        }
    }

    void MoveSelection(int direction) {
        selectedIndex += direction;

        const int optionCount = static_cast<int>(options.size());
        if (selectedIndex < 0) {
            selectedIndex = optionCount - 1;
        } else if (selectedIndex >= optionCount) {
            selectedIndex = 0;
        }
    }

    void ActivateSelection(StateManager& manager) {
        if (selectedIndex == 0) {
            manager.ChangeState<PlayState>();
        } else {
            CloseWindow();
        }
    }

    void DrawPixelArtPlaceholder() const {
        // 簡単なドット絵風の背景を描画
        const int tileSize = 16;
        for (int y = 0; y < GetScreenHeight(); y += tileSize) {
            for (int x = 0; x < GetScreenWidth(); x += tileSize) {
                const bool isEven = ((x / tileSize) + (y / tileSize)) % 2 == 0;
                const Color tileColor = isEven ? Color{25, 11, 60, 255} : Color{40, 7, 92, 255};
                DrawRectangle(x, y, tileSize, tileSize, tileColor);
            }
        }
        
    }

    void DrawBackground() const {
        if (background.id != 0) {
            const float screenWidth = static_cast<float>(GetScreenWidth());
            const float screenHeight = static_cast<float>(GetScreenHeight());
            const float scaleX = screenWidth / static_cast<float>(background.width);
            const float scaleY = screenHeight / static_cast<float>(background.height);
            const float scale = (scaleX < scaleY) ? scaleX : scaleY;

            const float destWidth = background.width * scale;
            const float destHeight = background.height * scale;
            const float offsetX = (screenWidth - destWidth) * 0.5f;
            const float offsetY = (screenHeight - destHeight) * 0.5f;

            const Rectangle src{0.0f, 0.0f, static_cast<float>(background.width), static_cast<float>(background.height)};
            const Rectangle dest{offsetX, offsetY, destWidth, destHeight};
        //    DrawTexturePro(background, src, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
        } else {
        //    DrawPixelArtPlaceholder();
        }
    }

    void DrawTitle() const {
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float baseY = 5.0f;
      //  const float offset = (std::sinf(titleAnimTime) * 0.5f + 0.5f) * titleYOffsetRange;
        const float offset = (0.5f) * titleYOffsetRange;

//        if (titleTexture.id != 0) {
//            const Rectangle src{0.0f, 0.0f, static_cast<float>(titleTexture.width), static_cast<float>(titleTexture.height)};
//            const float scale = (screenWidth * titleMaxWidthRatio) / static_cast<float>(titleTexture.width);
//            const Vector2 size{titleTexture.width * scale, titleTexture.height * scale};
//            const float x = (screenWidth - size.x) * 0.5f;
//            DrawTexturePro(titleTexture, src, Rectangle{x, baseY + offset, size.x, size.y}, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
//        } else {
            const char* fallbackTitle = "KINOKO SURVIVOR";
            const int titleFontSize = 48;
            const int titleWidth = MeasureText(fallbackTitle, titleFontSize);
            const int titleX = static_cast<int>((screenWidth - titleWidth) * 0.5f);
            DrawText(fallbackTitle, titleX, static_cast<int>(baseY + offset), titleFontSize, RAYWHITE);
//        }
    }

    void DrawMenu() const {
        const int menuFontSize = 32;
        const int spacing = 12;
        const int screenWidth = GetScreenWidth();

        const int baseY = 320;
        for (int i = 0; i < static_cast<int>(options.size()); ++i) {
            const Color color = (i == selectedIndex) ? Color{255, 255, 255, 255} : Color{180, 180, 200, 255};
            const int optionWidth = MeasureText(options[i], menuFontSize);
            const int optionX = (screenWidth - optionWidth) / 2;
            const int optionY = baseY + i * (menuFontSize + spacing);

            DrawText(options[i], optionX, optionY, menuFontSize, color);
            if (i == selectedIndex) {
                DrawRectangleLines(optionX - 20, optionY - 8, optionWidth + 40, menuFontSize + 16, color);
            }
        }
    }
};

} // namespace ks
