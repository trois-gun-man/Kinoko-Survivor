#pragma once

namespace ks {

class EventBus;

class UIResultScreen {
public:
    explicit UIResultScreen(EventBus& bus);
    void draw() const;
private:
    EventBus& m_bus;
};

} // namespace ks
