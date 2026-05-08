#include "console.hpp"

#include <stdint.h>

#include "../colors/framebuffer/framebuffer_colors.hpp"
#include "../text/text.hpp"
#include "../../drivers/video/framebuffer/framebuffer.hpp"
#include "../../../libk/string.hpp"
#include "../../../libk/memory.hpp"

namespace graphics::console {
    namespace {
        graphics::console::Renderer current_renderer;

        uint32_t cursor_x;
        uint32_t cursor_y;

        constexpr uint32_t padding          = 5;
        constexpr uint32_t character_width  = 8;
        constexpr uint32_t character_height = 8;
        constexpr uint32_t line_height      = character_height + padding;

        [[nodiscard]] uint32_t max_width() {
            return drivers::video::framebuffer::width;
        }

        [[nodiscard]] uint32_t max_height() {
            return drivers::video::framebuffer::height;
        }

        [[nodiscard]] bool needs_wrap() {
            return cursor_x + character_width > max_width() - padding;
        }

        [[nodiscard]] bool needs_scroll() {
            return cursor_y + character_height > max_height() - padding;
        }

        void scroll() {
            uint8_t* framebuffer =
                reinterpret_cast<uint8_t*>(drivers::video::framebuffer::address);

            const uint64_t scroll_amount =
                drivers::video::framebuffer::pitch * line_height;

            const uint64_t remaining_bytes =
                drivers::video::framebuffer::pitch *
                (max_height() - line_height);

            mem::move(
                framebuffer,
                framebuffer + scroll_amount,
                remaining_bytes
            );

            mem::set(
                framebuffer + remaining_bytes,
                0,
                scroll_amount
            );

            cursor_y -= line_height;
        }

        void newline() {
            cursor_x = padding;
            cursor_y += line_height;

            if (needs_scroll()) {
                scroll();
            }
        }
    }

    bool initialize(graphics::console::Renderer renderer) {
        current_renderer = renderer;

        cursor_x = padding;
        cursor_y = padding;

        return true;
    }

    void put_char(
        char c,
        graphics::colors::framebuffer::Color fg,
        graphics::colors::framebuffer::Color bg
    ) {
        (void)bg;

        if (c == '\n') {
            newline();
            return;
        }

        if (needs_wrap()) {
            newline();
        }

        graphics::text::draw_char(cursor_x, cursor_y, c, fg);

        cursor_x += character_width;
    }

    void print(
        const char* str,
        graphics::colors::framebuffer::Color fg,
        graphics::colors::framebuffer::Color bg
    ) {
        for (uint64_t i = 0; str[i] != '\0'; i++) {
            put_char(str[i], fg, bg);
        }
    }

    void println(
        const char* str,
        graphics::colors::framebuffer::Color fg,
        graphics::colors::framebuffer::Color bg
    ) {
        print(str, fg, bg);
        newline();
    }
}
