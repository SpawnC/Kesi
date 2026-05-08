#pragma once

namespace serial {

    /// @brief Initializes the serial port.
    /// @details Configures the COM1 serial port for text output communication.
    /// @note Currently initializes COM1 at 38400 baud, 8 data bits, no parity, and 1 stop bit.
    void init();

    /// @brief Writes a single character to the serial port.
    /// @param c The character to write.
    /// @details Waits until the transmitter is ready before sending the character.
    void write_char(char c);

    /// @brief Writes a null-terminated string to the serial port.
    /// @param str The string to write.
    /// @details Characters are sent one at a time using serial::write_char().
    /// @warning The input string must be null-terminated.
    void write(const char* str);

}