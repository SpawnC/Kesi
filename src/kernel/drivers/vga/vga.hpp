#pragma once

#include <stdint.h>

/// @brief VGA text mode driver for basic text output to the screen.
/// @details Provides functions to clear the screen, print strings, and print integers with specified foreground
///          and background colors using the VGA text mode buffer at 0xB8000.
/// @note VGA text mode supports a fixed resolution of 80x25 characters, with each character represented by
///       a 16-bit entry containing the ASCII character and its color attributes.
/// @deprecated Since the kernel has been mapped to the higher half,
///             direct access to the VGA buffer at 0xB8000 is no longer possible. 
///             This driver is no longer intended for active use, as the
///             kernel now relies on the framebuffer provided by the
///             bootloader for graphics output. 
///             The VGA memory region is currently inaccessible because
///             custom page mappings for 0xB8000 have not yet been
///             implemented. 
///             This driver will be kept for reference and potential
///             future use in a text mode console, but it is not expected
///             to be used in the current architecture.
namespace drivers::vga {

    enum class ColorFg : uint8_t {
        Black = 0,
        Blue = 1,
        Green = 2,
        Red = 4,
        LightGray = 7,
        Yellow = 14,
        White = 15,
    };

    enum class ColorBg : uint8_t {
        Black = 0,
        Blue = 1,
        Green = 2,
        Cyan = 3,
        Red = 4,
        Magenta = 5,
        Brown = 6,
        LightGray = 7,
    };



    /// @brief Creates a VGA color attribute from foreground and background colors.
    /// @details Combines foreground and background VGA colors into a single 8-bit color attribute.
    /// @param fg The foreground color (text color).
    /// @param bg The background color.
    /// @return An 8-bit VGA color attribute where:
    ///         - the lower nibble contains the foreground color
    ///         - the upper nibble contains the background color
    /// @note VGA text mode stores color attributes separately from character data.
    /// @note The background color is shifted left by 4 bits to occupy the upper nibble.
    /// @warning Both colors should be valid VGA color values for proper display.
    [[nodiscard]] constexpr uint8_t make_color(ColorFg fg, ColorBg bg) {
        return static_cast<uint8_t>(fg)
            | (static_cast<uint8_t>(bg) << 4);
    }

    /// @brief Combines a character and color attributes into a VGA entry.
    /// @details Combines a character and its color attributes into a single 16-bit VGA text mode entry.
    /// @param c The ASCII character to display.
    /// @param color The combined foreground and background color attributes.
    /// @return A 16-bit VGA entry where:
    ///         - the lower byte contains the ASCII character
    ///         - the upper byte contains the color attributes
    /// @note The lower byte stores the ASCII character,
    ///       while the upper byte stores the VGA color attributes.
    /// @warning The character should be a valid printable ASCII character for proper display.
    [[nodiscard]] constexpr uint16_t make_entry(char c, uint8_t color) {
        return static_cast<uint16_t>(c)
            | (static_cast<uint16_t>(color) << 8);
    }


    /// @brief Clears the screen by filling it with spaces using the specified background color.
    /// @param bg The background color to use for the cleared screen. Defaults to black.
    /// @note Resets the cursor position to the top-left corner.
    /// @warning This will overwrite all existing content on the screen.
    void clear(ColorBg bg = ColorBg::Black);

    /// @brief Prints a string to the VGA buffer.
    /// @details Prints a null-terminated string to the screen with the specified foreground and background colors.
    /// @param str The null-terminated string to print.
    /// @param fg The foreground color (text color). Defaults to white.
    /// @param bg The background color. Defaults to black.
    /// @note Does not append a newline.
    /// @warning Expects a valid null-terminated string.
    void print(const char* str,
               ColorFg fg = ColorFg::White,
               ColorBg bg = ColorBg::Black);


    /// @brief Prints a string followed by a newline to the VGA buffer.
    /// @details Prints a null-terminated string followed by a newline to the screen with the specified foreground and background colors.
    /// @param str The null-terminated string to print.
    /// @param fg The foreground color (text color). Defaults to white.
    /// @param bg The background color. Defaults to black.
    /// @note Appends a newline after printing the string.
    /// @warning Expects a valid null-terminated string. 
    
    void println(const char* str,
                 ColorFg fg = ColorFg::White,
                 ColorBg bg = ColorBg::Black);    
                 
    
    void printint(int value, ColorFg fg = ColorFg::White, ColorBg bg = ColorBg::Black);
}
