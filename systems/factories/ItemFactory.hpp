#pragma once

namespace ks {

class ItemBase;
class BuffStrategy;

class ItemFactory {
public:
    ItemBase* createRandomItem();
    BuffStrategy* createRandomBuff();
};

} // namespace ks
