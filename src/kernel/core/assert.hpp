#pragma once

#include "panic.hpp"

namespace kernel {

    /// @brief Verifies that a condition is true.

    /// @details
    /// This function checks whether the given condition evaluates to `true`.
    /// If the condition is `false`, the kernel enters a panic state by
    /// calling kernel::panic(), displaying the provided error message and
    /// halting the system.

    /// @param condition The condition to evaluate.
    /// @param msg The panic message displayed if the assertion fails.

    /// @note
    /// If the assertion succeeds, the function returns immediately and
    /// execution continues normally.

    /// @warning
    /// A failed assertion will permanently halt the kernel through
    /// kernel::panic().
    inline void assert(bool condition, const char* msg) {
        if (!condition) {
            kernel::panic(msg);
        }
    }

}