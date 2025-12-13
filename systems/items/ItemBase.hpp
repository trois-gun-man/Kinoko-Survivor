#pragma once

namespace ks {

class ItemBase {
public:
    virtual ~ItemBase() = default;
    virtual void apply();
};

} // namespace ks
