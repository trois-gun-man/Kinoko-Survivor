#pragma once

namespace ks {

class EventBus;

class UIHealthBar {
public:
    explicit UIHealthBar(EventBus& bus);
    void draw() const;
private:
    EventBus& m_bus;
};

} // namespace ks
