#pragma once

#include <stdint.h>

/// @brief Physical memory manager for the kernel.
/// @details Manages the allocation and deallocation of physical memory pages.
///          Provides functionality to track free and allocated pages, and handles
///          memory initialization from bootloader information.
namespace memory::physical {

    /// @brief Represents a single page in physical memory.
    /// @details A page is the smallest unit of memory that can be allocated or deallocated.
    ///          The standard page size is 4096 bytes on x86_64 architecture.
    using Page = uint8_t[4096];

    /// @brief Size of a single memory page in bytes.
    constexpr uint64_t PAGE_SIZE = 4096;

    /// @brief Initializes the physical memory manager.
    /// @details Sets up the memory manager using the memory map provided by the bootloader.
    ///          Must be called during kernel initialization before any memory allocation.
    /// @param memory_map_addr Address of the bootloader-provided memory map.
    /// @param memory_map_size Size of the memory map entries.
    /// @return true if initialization was successful, false otherwise.
    /// @note The memory map format is expected to be from the Limine bootloader.
    bool initialize(uint64_t memory_map_addr, uint64_t memory_map_size);

    /// @brief Allocates a single physical page of memory.
    /// @details Finds and reserves a free page from the memory pool.
    /// @return The physical address of the allocated page, or 0 if allocation fails.
    /// @note Returns 0 (nullptr) to indicate allocation failure.
    [[nodiscard]] uint64_t allocate_page();

    /// @brief Allocates multiple contiguous physical pages.
    /// @details Finds and reserves a contiguous block of free pages.
    /// @param page_count The number of consecutive pages to allocate.
    /// @return The physical address of the first allocated page, or 0 if allocation fails.
    /// @note Returns 0 (nullptr) to indicate allocation failure.
    /// @warning Allocating a large number of pages may fail if insufficient contiguous memory is available.
    [[nodiscard]] uint64_t allocate_pages(uint64_t page_count);

    /// @brief Deallocates a previously allocated physical page.
    /// @details Marks a page as free and makes it available for future allocations.
    /// @param page_addr The physical address of the page to deallocate.
    /// @note The address should be page-aligned (multiple of PAGE_SIZE).
    void free_page(uint64_t page_addr);

    /// @brief Deallocates multiple contiguous physical pages.
    /// @details Marks a block of pages as free and makes them available for future allocations.
    /// @param page_addr The physical address of the first page to deallocate.
    /// @param page_count The number of consecutive pages to deallocate.
    /// @note The address should be page-aligned (multiple of PAGE_SIZE).
    void free_pages(uint64_t page_addr, uint64_t page_count);

    /// @brief Returns the total amount of physical memory in the system.
    /// @details Returns the total usable physical memory as discovered from the bootloader memory map.
    /// @return Total physical memory in bytes.
    [[nodiscard]] uint64_t get_total_memory();

    /// @brief Returns the amount of currently free physical memory.
    /// @details Returns the amount of unallocated physical memory available for allocation.
    /// @return Free physical memory in bytes.
    [[nodiscard]] uint64_t get_free_memory();

    /// @brief Returns the amount of currently allocated physical memory.
    /// @details Returns the amount of allocated physical memory in use.
    /// @return Allocated physical memory in bytes.
    [[nodiscard]] uint64_t get_allocated_memory();

}
