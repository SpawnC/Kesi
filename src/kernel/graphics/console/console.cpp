#include "console.hpp"

#include <stdint.h>
#include <stdarg.h>

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
        
        
        graphics::colors::framebuffer::Color current_fg;
        graphics::colors::framebuffer::Color current_bg;
        
        
        volatile bool console_locked = false;

        constexpr uint32_t padding          = 5;
        constexpr uint32_t character_width  = 8;
        constexpr uint32_t character_height = 8;
        constexpr uint32_t line_height      = character_height + padding;
        constexpr uint32_t tab_size         = 4;

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
        
        void lock() {
            while (__sync_lock_test_and_set(&console_locked, 1)) {
                asm volatile("pause");
            }
        }
        
        void unlock() {
            __sync_lock_release(&console_locked);
        }
        
        void clear_line(uint32_t y) {
            uint8_t* framebuffer = reinterpret_cast<uint8_t*>(drivers::video::framebuffer::address);
            uint32_t line_start = y * drivers::video::framebuffer::pitch;
            uint32_t line_bytes = drivers::video::framebuffer::pitch;
            
            for (uint32_t i = 0; i < line_bytes; i += 4) {
                *reinterpret_cast<uint32_t*>(framebuffer + line_start + i) = 
                    static_cast<uint32_t>(current_bg);
            }
        }

        void scroll() {
            if (cursor_y < line_height) {
                return; 
            }
            
            uint8_t* framebuffer = reinterpret_cast<uint8_t*>(drivers::video::framebuffer::address);
            const uint64_t scroll_amount = drivers::video::framebuffer::pitch * line_height;
            const uint64_t remaining_bytes = drivers::video::framebuffer::pitch * (max_height() - line_height);

            mem::move(framebuffer, framebuffer + scroll_amount, remaining_bytes);
            
            for (uint32_t i = 0; i < scroll_amount; i += 4) {
                *reinterpret_cast<uint32_t*>(framebuffer + remaining_bytes + i) = 
                    static_cast<uint32_t>(current_bg);
            }

            cursor_y -= line_height;
        }

        void newline() {
            cursor_x = padding;
            cursor_y += line_height;

            if (needs_scroll()) {
                scroll();
            }
        }
        
        void carriage_return() {
            cursor_x = padding;
        }
        
        void backspace() {
            if (cursor_x > padding) {
                cursor_x -= character_width;
                graphics::text::draw_char(cursor_x, cursor_y, ' ', current_fg, current_bg);
            }
        }
        
        void tab() {
            for (uint32_t i = 0; i < tab_size; i++) {
                put_char(' ', current_fg, current_bg);
            }
        }
        
        void print_int(int64_t num) {
            if (num < 0) {
                put_char('-', current_fg, current_bg);
                num = -num;
            }
            
            char buffer[32];
            int i = 0;
            do {
                buffer[i++] = '0' + (num % 10);
                num /= 10;
            } while (num > 0);
            
            while (i > 0) {
                put_char(buffer[--i], current_fg, current_bg);
            }
        }
        
        void print_hex(uint64_t num) {
            put_char('0', current_fg, current_bg);
            put_char('x', current_fg, current_bg);
            
            bool leading_zero = true;
            for (int i = 60; i >= 0; i -= 4) {
                uint8_t nibble = (num >> i) & 0xF;
                if (nibble != 0 || !leading_zero || i == 0) {
                    leading_zero = false;
                    char c = (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
                    put_char(c, current_fg, current_bg);
                }
            }
        }
        
        void print_uint(uint64_t num) {
            char buffer[32];
            int i = 0;
            do {
                buffer[i++] = '0' + (num % 10);
                num /= 10;
            } while (num > 0);
            
            while (i > 0) {
                put_char(buffer[--i], current_fg, current_bg);
            }
        }
    }

    bool initialize(graphics::console::Renderer renderer) {
        current_renderer = renderer;
        current_fg = graphics::colors::framebuffer::Color::White;
        current_bg = graphics::colors::framebuffer::Color::Black;
        cursor_x = padding;
        cursor_y = padding;
        return true;
    }
    
    void set_colors(graphics::colors::framebuffer::Color fg, 
                    graphics::colors::framebuffer::Color bg) {
        current_fg = fg;
        current_bg = bg;
    }
    
    void clear() {
        lock();
        
        uint8_t* framebuffer = reinterpret_cast<uint8_t*>(drivers::video::framebuffer::address);
        uint64_t total_bytes = drivers::video::framebuffer::pitch * max_height();
        
        for (uint64_t i = 0; i < total_bytes; i += 4) {
            *reinterpret_cast<uint32_t*>(framebuffer + i) = static_cast<uint32_t>(current_bg);
        }
        
        cursor_x = padding;
        cursor_y = padding;
        
        unlock();
    }
    
    void put_char(char c) {
        put_char(c, current_fg, current_bg);
    }

    void put_char(char c, 
                  graphics::colors::framebuffer::Color fg,
                  graphics::colors::framebuffer::Color bg) {
        lock();
        
        switch (c) {
            case '\n':
                newline();
                unlock();
                return;
            case '\r':
                carriage_return();
                unlock();
                return;
            case '\t':
                tab();
                unlock();
                return;
            case '\b':
                backspace();
                unlock();
                return;
            default:
                break;
        }
        
        if (needs_wrap()) {
            newline();
        }
        
        graphics::text::draw_char(cursor_x, cursor_y, c, fg, bg);
        cursor_x += character_width;
        
        unlock();
    }

    void print(const char* str) {
        print(str, current_fg, current_bg);
    }
    
    void print(const char* str,
               graphics::colors::framebuffer::Color fg,
               graphics::colors::framebuffer::Color bg) {
        lock();
        for (uint64_t i = 0; str[i] != '\0'; i++) {
            put_char(str[i], fg, bg);
        }
        unlock();
    }

    void println(const char* str) {
        println(str, current_fg, current_bg);
    }
    
    void println(const char* str,
                 graphics::colors::framebuffer::Color fg,
                 graphics::colors::framebuffer::Color bg) {
        print(str, fg, bg);
        newline();
    }
    
    void print_int(int64_t num) {
        lock();
        print_int(num);
        unlock();
    }
    
    void print_hex(uint64_t num) {
        lock();
        print_hex(num);
        unlock();
    }
    
    void print_uint(uint64_t num) {
        lock();
        print_uint(num);
        unlock();
    }
    
    void printf(const char* format, ...) {
        lock();
        
        va_list args;
        va_start(args, format);
        
        for (const char* p = format; *p; p++) {
            if (*p == '%' && *(p + 1)) {
                p++;
                switch (*p) {
                    case 's': {
                        const char* str = va_arg(args, const char*);
                        print(str);
                        break;
                    }
                    case 'd': {
                        int64_t num = va_arg(args, int64_t);
                        print_int(num);
                        break;
                    }
                    case 'u': {
                        uint64_t num = va_arg(args, uint64_t);
                        print_uint(num);
                        break;
                    }
                    case 'x': {
                        uint64_t num = va_arg(args, uint64_t);
                        print_hex(num);
                        break;
                    }
                    case 'c': {
                        char c = static_cast<char>(va_arg(args, int));
                        put_char(c);
                        break;
                    }
                    case '%': {
                        put_char('%');
                        break;
                    }
                    default: {
                        put_char('%');
                        put_char(*p);
                        break;
                    }
                }
            } else {
                put_char(*p);
            }
        }
        
        va_end(args);
        unlock();
    }
    
    void set_cursor_position(uint32_t x, uint32_t y) {
        lock();
        
        if (x <= max_width() - character_width) {
            cursor_x = x;
        }
        if (y <= max_height() - character_height) {
            cursor_y = y;
        }
        
        unlock();
    }
    
    void get_cursor_position(uint32_t& x, uint32_t& y) {
        lock();
        x = cursor_x;
        y = cursor_y;
        unlock();
    }
}
