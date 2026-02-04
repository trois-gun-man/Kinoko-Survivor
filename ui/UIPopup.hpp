#pragma once

namespace ks {

class EventBus;

class UIPopup {
public:
    explicit UIPopup(EventBus& bus);
    void show(const char* message);
private:
    EventBus& m_bus;
};

} // namespace ks
