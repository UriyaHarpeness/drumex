#pragma once

namespace ext {
    template<typename C, typename K>
    bool exists(C c, K k) {
        return (c.find(k) != c.end());
    }
}