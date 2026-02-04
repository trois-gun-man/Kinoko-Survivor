#pragma once

namespace ks {

class Enemy;

class AIStrategy {
public:
    virtual ~AIStrategy() = default;
    virtual void decideAction(Enemy& owner, float dt) = 0;
};

} // namespace ks
