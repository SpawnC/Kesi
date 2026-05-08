#pragma once

#include <stdint.h>

namespace io {

    /// @brief Writes a byte to the specified I/O port.
    /// @details Sends an 8-bit value to a hardware I/O port using the x86 OUT instruction.
    /// @param port The I/O port address to write to.
    /// @param value The 8-bit value to write.
    /// @note This function is architecture-specific and only works on x86/x86_64 systems.
    inline void outb(uint16_t port, uint8_t value) {

        asm volatile("outb %0, %1"
                    :
                    : "a"(value), "Nd"(port));
    }

    /// @brief Reads a byte from the specified I/O port.
    /// @details Retrieves an 8-bit value from a hardware I/O port using the x86 IN instruction.
    /// @param port The I/O port address to read from.
    /// @return The 8-bit value read from the port.
    /// @note This function is architecture-specific and only works on x86/x86_64 systems.
    [[nodiscard]] inline uint8_t inb(uint16_t port) {

        uint8_t ret;

        asm volatile("inb %1, %0"
                    : "=a"(ret)
                    : "Nd"(port));

        return ret;
    }

}