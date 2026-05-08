#pragma once

#include <stdint.h>

#include "../colors/framebuffer/framebuffer_colors.hpp"

/// @brief Text rendering utilities for the kernel's graphics subsystem.
/// @details This namespace contains functions and utilities for rendering text to the screen using the kernel's graphics subsystem.
///          It provides functionality for drawing characters and strings using bitmap fonts, as well as managing text rendering state such as cursor position and text color.
/// @note This module relies on the framebuffer driver for rendering text to the screen, and may also utilize bitmap font data defined in the graphics::font namespace.
/// @warning Make sure that the framebuffer is properly initialized before using any text rendering functions, as they depend on the framebuffer for output. 
///          Additionally, ensure that the appropriate font data is available for rendering characters correctly.
namespace graphics::text {

    inline constexpr uint32_t character_width  = 8;
    inline constexpr uint32_t character_height = 8;

    /// @brief Draws a single character at the specified coordinates with the given color.
    /// @details Renders the character `c` at the position (x, y) on the screen using the specified color.
    ///         The character is rendered using a bitmap font, where each character is represented as an 8x8 pixel grid. 
    ///         The color format and supported color depth depend on the specific framebuffer implementation and hardware capabilities.
    /// @param x The x-coordinate of the top-left corner where the character will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param y The y-coordinate of the top-left corner where the character will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param c The character to be drawn. This should be a valid ASCII character (0-127) that corresponds to the bitmap font data.
    /// @param color The color value to use for rendering the character, the color format depends on the framebuffer pixel format provided by the bootloader.
    /// @param scale The scaling factor for rendering the character. A value of 1 means no scaling, while values greater than 1 will enlarge the character.
    void draw_char(uint32_t x, uint32_t y, char c, uint32_t color, uint32_t scale = 1);

    /// @brief Draws a single character at the specified coordinates with the given color.
    /// @details Renders the character `c` at the position (x, y) on the screen using the specified color.
    ///         The character is rendered using a bitmap font, where each character is represented as an 8x8 pixel grid. 
    ///         The color format and supported color depth depend on the specific framebuffer implementation and hardware capabilities.
    /// @param x The x-coordinate of the top-left corner where the character will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param y The y-coordinate of the top-left corner where the character will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param c The character to be drawn. This should be a valid ASCII character (0-127) that corresponds to the bitmap font data.
    /// @param color The color value to use for ren dering the character, specified as a Color enum value from graphics::colors::framebuffer::Color.
    /// @param scale The scaling factor for rendering the character. A value of 1 means no scaling, while values greater than 1 will enlarge the character.
    void draw_char(uint32_t x, uint32_t y, char c, graphics::colors::framebuffer::Color color, uint32_t scale = 1);

    /// @brief Draws a null-terminated string at the specified coordinates with the given color.
    /// @details Renders the null-terminated string `str`
    ///          starting at the position (x, y) on the screen
    ///          using the specified color.
    /// @param x The x-coordinate of the top-left corner where the first character of the string will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param y The y-coordinate of the top-left corner where the first character of the
    ///          string will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param str The null-terminated string to be drawn. Each character in the string should be a valid ASCII character (0-127) that corresponds to the bitmap font data.
    /// @param color The color value to use for rendering the text, the color format depends on the framebuffer pixel format provided by the bootloader.
    /// @param scale The scaling factor for rendering the string. A value of 1 means no scaling, while values greater than 1 will enlarge the characters.
    /// @note The function will continue to render characters until it encounters a null terminator ('\0') in the string.
    void draw_string(uint32_t x, uint32_t y, const char* str, uint32_t color, uint32_t scale = 1);


    /// @brief Draws a null-terminated string at the specified coordinates with the given color.
    /// @details Renders the null-terminated string `str`
    ///          starting at the position (x, y) on the screen
    ///          using the specified color.
    /// @param x The x-coordinate of the top-left corner where the first character of the string will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param y The y-coordinate of the top-left corner where the first character of the
    ///          string will be drawn. The origin (0, 0) is located at the top-left corner of the framebuffer.
    /// @param str The null-terminated string to be drawn. Each character in the string should be a valid ASCII character (0-127) that corresponds to the bitmap font data.
    /// @param color The color value to use for rendering the text, specified as a Color enum value from graphics::colors::framebuffer::Color.
    /// @param scale The scaling factor for rendering the string. A value of 1 means no scaling, while values greater than 1 will enlarge the characters.
    /// @note The function will continue to render characters until it encounters a null terminator ('\0') in the string.
    void draw_string(uint32_t x, uint32_t y, const char* str, graphics::colors::framebuffer::Color color, uint32_t scale = 1);

}