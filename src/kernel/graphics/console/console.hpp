#pragma once

#include "../colors/framebuffer/framebuffer_colors.hpp"

namespace graphics::console {

    // only Framebuffer for now, but we can add more renderers in the future (e.g. VGA text mode)
    enum class Renderer {
        Framebuffer
    };


    /// @brief Initializes the console with the specified renderer.
    /// @details Sets up the console for text output using the specified rendering method.
    /// @param renderer The renderer to use for console output.
    /// @return True if initialization was successful, false otherwise.
    bool initialize(Renderer renderer = Renderer::Framebuffer);

    /// @brief Outputs a single character to the console.
    /// @param c The character to output.
    /// @param fg The foreground color.
    /// @param bg The background color.
    /// @todo rewrite graphics::text::text.hpp and graphics::text::text.cpp to take in background colors
    // TODO: above
    void put_char(char c, 
        graphics::colors::framebuffer::Color fg = graphics::colors::framebuffer::Color::White, 
        graphics::colors::framebuffer::Color bg = graphics::colors::framebuffer::Color::Black);

    /// @brief Outputs a string to the console.
    /// @param str The string to output.
    /// @param fg The foreground color.
    /// @param bg The background color.
    void print(const char* str, 
        graphics::colors::framebuffer::Color fg = graphics::colors::framebuffer::Color::White, 
        graphics::colors::framebuffer::Color bg = graphics::colors::framebuffer::Color::Black);

    /// @brief Outputs a string to the console followed by a newline.
    /// @param str The string to output.
    /// @param fg The foreground color.
    /// @param bg The background color.
    void println(const char* str, 
        graphics::colors::framebuffer::Color fg = graphics::colors::framebuffer::Color::White, 
        graphics::colors::framebuffer::Color bg = graphics::colors::framebuffer::Color::Black);
}