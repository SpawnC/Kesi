#pragma once

#include <stdint.h>

namespace mem {

    inline void* mcpy(void* dest,
                        const void* src,
                        uint64_t size) {

        auto* d = static_cast<uint8_t*>(dest);
        auto* s = static_cast<const uint8_t*>(src);

        for (uint64_t i = 0; i < size; i++) {
            d[i] = s[i];
        }

        return dest;
    }

    inline void* move(void* dest,
                         const void* src,
                         uint64_t size) {

        auto* d = static_cast<uint8_t*>(dest);
        auto* s = static_cast<const uint8_t*>(src);

        if (d < s) {

            for (uint64_t i = 0; i < size; i++) {
                d[i] = s[i];
            }

        } else {

            for (uint64_t i = size; i != 0; i--) {
                d[i - 1] = s[i - 1];
            }
        }

        return dest;
    }

    inline void* set(void* dest,
                        int value,
                        uint64_t size) {

        auto* d = static_cast<uint8_t*>(dest);

        for (uint64_t i = 0; i < size; i++) {
            d[i] = static_cast<uint8_t>(value);
        }

        return dest;
    }

    inline int cmp(const void* a,
                      const void* b,
                      uint64_t size) {

        auto* aa = static_cast<const uint8_t*>(a);
        auto* bb = static_cast<const uint8_t*>(b);

        for (uint64_t i = 0; i < size; i++) {

            if (aa[i] != bb[i]) {
                return aa[i] - bb[i];
            }
        }

        return 0;
    }
}