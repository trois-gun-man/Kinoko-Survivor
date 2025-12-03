#pragma once

namespace ks {

class EventBus;

class AudioManager {
public:
    explicit AudioManager(EventBus& bus);
    void update(float dt);
private:
    EventBus& m_bus;
};

} // namespace ks
