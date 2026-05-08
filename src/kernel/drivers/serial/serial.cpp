#include "serial.hpp"

#include "../../arch/x86_64/io.hpp"

#include <stdint.h>

namespace serial {

    constexpr uint16_t COM1 = 0x3F8;

    static bool is_transmit_empty() {
        return io::inb(COM1 + 5) & 0x20;
    }

    void init() {

        io::outb(COM1 + 1, 0x00);

        io::outb(COM1 + 3, 0x80);

        io::outb(COM1 + 0, 0x03);
        io::outb(COM1 + 1, 0x00);

        io::outb(COM1 + 3, 0x03);

        io::outb(COM1 + 2, 0xC7);

        io::outb(COM1 + 4, 0x0B);
    }

    void write_char(char c) {

        while (!is_transmit_empty());

        io::outb(COM1, c);
    }

    void write(const char* str) {

        while (*str) {

            write_char(*str);

            str++;
        }
    }

}