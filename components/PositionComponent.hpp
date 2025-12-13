#pragma once

namespace ks {

class PositionComponent {
public:
    void set(float x, float y);
    void translate(float dx, float dy);
private:
    float m_x = 0.0f;
    float m_y = 0.0f;
};

} // namespace ks
