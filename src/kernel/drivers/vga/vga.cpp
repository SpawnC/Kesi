#include "vga.hpp"

namespace drivers::vga {

    namespace {
        constexpr int WIDTH  = 80;
        constexpr int HEIGHT = 25;

        volatile uint16_t* const buffer = reinterpret_cast<uint16_t*>(0xB8000);

        int row = 0;
        int col = 0;

        inline void advance_cursor() {
            if (++col >= WIDTH) {
                col = 0;
                if (++row >= HEIGHT) {
                    row = 0; // TODO: Implement scrolling instead of wrapping around
                }
            }
        }

        inline void put_char_internal(char c, uint8_t color) {
            if (c == '\n') {
                col = 0;
                if (++row >= HEIGHT) row = 0;
                return;
            }

            buffer[row * WIDTH + col] = make_entry(c, color);
            advance_cursor();
        }
    }

    void clear(ColorBg bg) {
        const uint8_t color = make_color(ColorFg::White, bg);

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                buffer[y * WIDTH + x] = make_entry(' ', color);
            }
        }

        row = 0;
        col = 0;
    }

    void print(const char* str, ColorFg fg, ColorBg bg) {
        const uint8_t color = make_color(fg, bg);

        for (const char* p = str; *p; ++p) {
            put_char_internal(*p, color);
        }
    }

    void println(const char* str, ColorFg fg, ColorBg bg) {
        print(str, fg, bg);
        put_char_internal('\n', make_color(fg, bg));
    }

}