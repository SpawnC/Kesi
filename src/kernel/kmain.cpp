#include "drivers/vga/vga.hpp"
#include "core/panic.hpp"
#include "core/assert.hpp"
#include "../libk/string.hpp"
#include "graphics/colors/framebuffer/framebuffer_colors.hpp"
#include "drivers/video/framebuffer/framebuffer.hpp"
#include "graphics/text/text.hpp"
#include "graphics/fonts/font8x8.hpp"
#include "graphics/console/console.hpp"
#include "drivers/serial/serial.hpp"

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

    drivers::video::framebuffer::clear();

    if (!graphics::console::initialize()) {
        kernel::panic("Failed to initialize console");
    }

    serial::init();


    while (true) {
        asm volatile("hlt");
    }
}

extern "C" void _start() {
    kmain();
}