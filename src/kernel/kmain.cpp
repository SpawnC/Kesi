#include "drivers/vga/vga.hpp"
#include "core/panic.hpp"
#include "core/assert.hpp"
#include "../libk/string.hpp"
#include "drivers/video/framebuffer/framebuffer.hpp"
#include "graphics/text/text.hpp"
#include "graphics/fonts/font8x8.hpp"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[3] = {
    0xf9562b2d5c95a6c8,
    0x6a7b384944536bdc,
    0
};

[[noreturn]] void kmain() {

    if (!drivers::video::framebuffer::initialize()) {
        kernel::panic("Failed to initialize framebuffer");
    }

    drivers::video::framebuffer::clear(graphics::colors::framebuffer::Color::White);

    graphics::text::draw_string(10, 10, "Kesi Kernel initialized", graphics::colors::framebuffer::Color::Red, 5);

    while (true) {
        asm volatile("hlt");
    }
}

extern "C" void _start() {
    kmain();
}