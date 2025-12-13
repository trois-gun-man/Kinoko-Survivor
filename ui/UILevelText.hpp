#pragma once

namespace ks {

class EventBus;

class UILevelText {
public:
    explicit UILevelText(EventBus& bus);
    void draw() const;
private:
    EventBus& m_bus;
};

} // namespace ks
