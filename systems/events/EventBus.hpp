#pragma once

namespace ks {

class EventBus {
public:
    void subscribe(int eventId, void(*callback)(void*));
    void publish(int eventId, void* payload);
};

} // namespace ks
