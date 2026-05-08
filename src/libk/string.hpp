#pragma once

#include <cstddef>

namespace str {

    /// @brief Calculates the length of a null-terminated string.
    /// @param str A pointer to the null-terminated string.
    /// @return The number of characters in the string, excluding the null terminator.
    /// @note The function iterates through the string until it finds the null terminator ('\0').
    /// @warning The input string must be null-terminated to avoid undefined behavior.
    [[nodiscard]] inline size_t strlen(const char* str) {
        size_t len = 0;
        while (str[len]) {
            ++len;
        }
        return len;
    }

    /// @brief Converts an integer to a null-terminated string.
    /// @param value The integer value to convert.
    /// @param buffer The destination buffer.
    /// @return Pointer to the resulting string buffer.
    /// @note Buffer must be large enough to hold the result.
    /// @warning Only supports base-10 conversion.
    inline char* itoa(int value, char* buffer) {

        if (buffer == nullptr) {
            return nullptr;
        }

        int i = 0;
        bool negative = false;

        // Handle negative numbers
        if (value < 0) {
            negative = true;
            value = -value;
        }

        // Handle zero
        if (value == 0) {
            buffer[i++] = '0';
        } else {

            // Build digits in reverse
            while (value > 0) {
                buffer[i++] = '0' + (value % 10);
                value /= 10;
            }

            // Add negative sign
            if (negative) {
                buffer[i++] = '-';
            }
        }

        buffer[i] = '\0';

        // Reverse string
        for (int j = 0; j < i / 2; ++j) {
            char temp = buffer[j];
            buffer[j] = buffer[i - j - 1];
            buffer[i - j - 1] = temp;
        }

        return buffer;
    }    

}