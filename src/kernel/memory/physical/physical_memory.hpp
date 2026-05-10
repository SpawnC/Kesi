#pragma once

#include <stdint.h>
#include <stddef.h>

namespace memory::physical {

    /// @brief Represents the state of a physical memory page.
    enum class PageState : uint8_t {
        Free,       ///< Page is available for allocation
        Used,       ///< Page is in use
        Reserved    ///< Page is reserved (e.g., for kernel or hardware)
    };

    /// @brief Bitmap-based physical memory manager.
    /// @details Manages physical memory by tracking the state of each page using a bitmap.
    ///          This implementation uses a simple bitmap approach where each bit represents
    ///          a single page. A bit value of 0 indicates a free page, while 1 indicates
    ///          a page in use or reserved.
    class PhysicalMemoryManager {
    public:
        static constexpr uint64_t PAGE_SIZE = 4096;       ///< Standard page size (4 KiB)
        static constexpr uint64_t BITMAP_ENTRY_BITS = 64; ///< Bits per uint64_t entry

        /// @brief Initializes the physical memory manager.
        /// @details Sets up the bitmap and marks memory regions as appropriate.
        /// @param total_memory Total physical memory in bytes.
        /// @param kernel_end Physical address where the kernel ends.
        void initialize(uint64_t total_memory, uint64_t kernel_end);

        /// @brief Allocates a single page of physical memory.
        /// @return Physical address of the allocated page, or 0 if allocation fails.
        [[nodiscard]] uint64_t allocate_page();

        /// @brief Allocates multiple contiguous pages of physical memory.
        /// @param count Number of contiguous pages to allocate.
        /// @return Physical address of the first page, or 0 if allocation fails.
        [[nodiscard]] uint64_t allocate_pages(uint64_t count);

        /// @brief Frees a single page of physical memory.
        /// @param physical_address Physical address of the page to free.
        void free_page(uint64_t physical_address);

        /// @brief Frees multiple contiguous pages of physical memory.
        /// @param physical_address Physical address of the first page to free.
        /// @param count Number of contiguous pages to free.
        void free_pages(uint64_t physical_address, uint64_t count);

        /// @brief Returns the total number of physical memory pages.
        /// @return Total number of pages.
        [[nodiscard]] uint64_t total_pages() const;

        /// @brief Returns the number of free physical memory pages.
        /// @return Number of free pages.
        [[nodiscard]] uint64_t free_pages() const;

        /// @brief Returns the number of used physical memory pages.
        /// @return Number of used pages.
        [[nodiscard]] uint64_t used_pages() const;

        /// @brief Checks if a page is allocated.
        /// @param physical_address Physical address of the page to check.
        /// @return True if the page is allocated, false otherwise.
        [[nodiscard]] bool is_page_allocated(uint64_t physical_address) const;

        /// @brief Converts a physical address to a page index.
        /// @param physical_address Physical address.
        /// @return Page index.
        [[nodiscard]] static uint64_t address_to_page_index(uint64_t physical_address);

        /// @brief Converts a page index to a physical address.
        /// @param page_index Page index.
        /// @return Physical address of the page.
        [[nodiscard]] static uint64_t page_index_to_address(uint64_t page_index);

    private:
        uint64_t total_memory_bytes;
        uint64_t total_num_pages;
        uint64_t used_page_count;
        uint64_t* bitmap;

        /// @brief Finds the first free page starting from a given index.
        /// @param start_index Starting page index to search from.
        /// @return Page index of the first free page, or total_num_pages if not found.
        [[nodiscard]] uint64_t find_free_page(uint64_t start_index = 0) const;

        /// @brief Finds a sequence of free contiguous pages.
        /// @param count Number of contiguous pages to find.
        /// @return Page index of the first page in the sequence, or total_num_pages if not found.
        [[nodiscard]] uint64_t find_free_pages(uint64_t count) const;

        /// @brief Sets the state of a page in the bitmap.
        /// @param page_index Page index.
        /// @param allocated True to mark as allocated, false to mark as free.
        void set_page_allocated(uint64_t page_index, bool allocated);

        /// @brief Gets the state of a page in the bitmap.
        /// @param page_index Page index.
        /// @return True if the page is allocated, false otherwise.
        [[nodiscard]] bool get_page_allocated(uint64_t page_index) const;
    };

    /// @brief Global instance of the physical memory manager.
    extern PhysicalMemoryManager g_pmm;

}
