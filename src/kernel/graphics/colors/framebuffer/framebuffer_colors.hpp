#pragma once

#include <stdint.h>

namespace graphics::colors::framebuffer {

    enum class Color : uint32_t {
        Black       = 0x000000,
        White       = 0xFFFFFF,
        Red         = 0xFF0000,
        Green       = 0x00FF00,
        Blue        = 0x0000FF,
        Yellow      = 0xFFFF00,
        Cyan        = 0x00FFFF,
        Magenta     = 0xFF00FF,
        Gray        = 0x808080,
        LightGray   = 0xC0C0C0,
        DarkGray    = 0x404040
    };



    [[nodiscard]] constexpr uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        return (static_cast<uint32_t>(r) << 16) |
            (static_cast<uint32_t>(g) << 8)  |
                static_cast<uint32_t>(b);
    }

}