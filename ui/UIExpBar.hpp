#pragma once

namespace ks {

class EventBus;

class UIExpBar {
public:
    explicit UIExpBar(EventBus& bus);
    void draw() const;
private:
    EventBus& m_bus;
};

} // namespace ks
