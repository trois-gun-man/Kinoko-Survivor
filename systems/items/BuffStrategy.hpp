#pragma once

namespace ks {

class Entity;

class BuffStrategy {
public:
    virtual ~BuffStrategy() = default;
    virtual void apply(Entity& target) = 0;
};

} // namespace ks
