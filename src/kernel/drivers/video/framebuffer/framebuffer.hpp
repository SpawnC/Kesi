#pragma once

#include "../../../graphics/colors/framebuffer/framebuffer_colors.hpp"

#include <stdint.h> 

namespace drivers::video::framebuffer {

    extern uint64_t width;
    extern uint64_t height;

    /// @brief Initializes the framebuffer for graphics output.
    /// @details Sets up the framebuffer based on information provided by the bootloader,
    ///          allowing the kernel to render graphics and text to the screen.
    /// @note This function should be called during kernel initialization after receiving
    ///       framebuffer information from the bootloader.
    /// @return true if the framebuffer was successfully initialized, false otherwise.
    bool initialize();


    /// @brief Clears the framebuffer by filling it with a default background color.
    /// @details Fills the entire framebuffer with a default background color, effectively
    ///          clearing any existing content on the screen.
    /// @warning This function assumes that the framebuffer has already been initialized and is ready for use.
    void clear(graphics::colors::framebuffer::Color color = graphics::colors::framebuffer::Color::Black);


    /// @brief Puts a pixel at the specified coordinates with the given color.
    /// @details Sets the pixel at (x, y) to the specified color in the framebuffer.
    /// @param x x-coordinate of the pixel to set
    /// @param y y-coordinate of the pixel to set
    /// @param color color value to set the pixel to, the color format depends on the framebuffer pixel format provided by the bootloader.
    /// @note The color format and supported color depth depend on the specific framebuffer implementation and hardware capabilities.
    /// @note The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @warning Ensure that (x, y) coordinates are within the bounds of the framebuffer. If they are not, the function returns without doing anything.
    void put_pixel(uint32_t  x, uint32_t  y, uint32_t color);


}