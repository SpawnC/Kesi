#include "text.hpp"

#include "../../drivers/video/framebuffer/framebuffer.hpp"
#include "../fonts/font8x8.hpp"


namespace graphics::text {

    void draw_char(uint32_t x, uint32_t y, char c, uint32_t color, uint32_t scale) {

        uint8_t character = static_cast<uint8_t>(c);

        if (character > 127) return;

        const uint8_t* bitmap = font::font8x8_basic[character];

        for (uint32_t row = 0; row < character_height; row++) {
            for (uint32_t col = 0; col < character_width; col++) {
                if (bitmap[row] & (1 << col)) {
                    for (uint32_t sy = 0; sy < scale; sy++) {
                        for (uint32_t sx = 0; sx < scale; sx++) {
                            drivers::video::framebuffer::put_pixel(x + col * scale + sx, y + row * scale + sy, color);
                        }
                    }
                }
            }
        }
    }

    void draw_char(uint32_t x, uint32_t y, char c, graphics::colors::framebuffer::Color color, uint32_t scale) {
        draw_char(x, y, c, static_cast<uint32_t>(color), scale);
    }

    void draw_string(uint32_t x, uint32_t y, const char* str, uint32_t color, uint32_t scale) {

        uint32_t cursor_x = x;
        uint32_t cursor_y = y;

        while (*str) {
            if (*str == '\n') {
                cursor_x = x;
                cursor_y += character_height * scale;
            } else {
                draw_char(cursor_x, cursor_y, *str, color, scale);
                cursor_x += character_width * scale;
            }
            str++;
        }
    }

    void draw_string(uint32_t x, uint32_t y, const char* str, graphics::colors::framebuffer::Color color, uint32_t scale) {
        draw_string(x, y, str, static_cast<uint32_t>(color), scale);
    }


}