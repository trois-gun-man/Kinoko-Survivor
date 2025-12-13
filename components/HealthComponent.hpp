#pragma once

namespace ks {

class HealthComponent {
public:
    void setMax(int value);
    void applyDamage(int value);
    bool isDead() const;
private:
    int m_current = 0;
    int m_max = 0;
};

} // namespace ks
