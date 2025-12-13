#pragma once

namespace ks {

class RenderComponent {
public:
    void loadSprite(const char* path);
    void draw() const;
};

} // namespace ks
