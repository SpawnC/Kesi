#pragma once

#include "../graphics/colors/framebuffer/framebuffer_colors.hpp"
#include "../graphics/text/text.hpp"
#include "../drivers/video/framebuffer/framebuffer.hpp"


namespace kernel {
    /// @brief Halts the system and displays a panic message on the screen.
    /// @details This function is called when the kernel encounters a critical error from which it cannot recover.
    ///          It clears the screen with a red background, displays the panic message, disables interrupts, and halts the CPU to prevent further execution.
    /// @param message The panic message to display.
    /// @warning This function does not return and will halt the system indefinitely. It should only be called in critical failure scenarios.
    [[noreturn]] inline void panic(const char* message) {  
        asm volatile("cli");

        drivers::video::framebuffer::clear(graphics::colors::framebuffer::Color::Red);
        graphics::text::draw_string(10, 10, message, graphics::colors::framebuffer::Color::White, 2);

        while (true) {
            asm volatile("hlt");
        }
    }
}