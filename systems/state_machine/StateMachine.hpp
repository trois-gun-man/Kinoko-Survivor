#pragma once

#include "IState.hpp"

namespace ks {

class StateMachine {
public:
    void changeState(IState* next);
    void update(float dt);
private:
    IState* m_current = nullptr;
};

} // namespace ks
