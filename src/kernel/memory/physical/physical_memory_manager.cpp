#include "physical_memory_manager.hpp"
#include "../../core/assert.hpp"
#include "../../core/panic.hpp"
#include "../../../libk/memory.hpp"
#include <limine.h>

namespace memory::physical {

    namespace {
        /// @brief Bitmap entry type for tracking page allocation status.
        /// Each bit represents one page: 1 = allocated, 0 = free
        using BitmapEntry = uint8_t;

        /// @brief Number of pages per bitmap entry (bits)
        constexpr uint64_t PAGES_PER_ENTRY = 8;

        /// @brief Maximum number of pages the manager can handle
        constexpr uint64_t MAX_PAGES = 1024 * 1024;  // 4GB with 4KB pages

        /// @brief Bitmap for tracking page allocation
        BitmapEntry bitmap[MAX_PAGES / PAGES_PER_ENTRY] = {};

        /// @brief Total number of pages in the system
        uint64_t total_pages = 0;

        /// @brief Number of allocated pages
        uint64_t allocated_pages = 0;

        /// @brief Limine memory map request
        __attribute__((used, section(".requests")))
        volatile limine_memmap_request memmap_request = {
            LIMINE_MEMMAP_REQUEST_ID,
            0,
            nullptr
        };

        /// @brief Sets a page as allocated in the bitmap
        void set_page_allocated(uint64_t page_index) {
            uint64_t entry = page_index / PAGES_PER_ENTRY;
            uint64_t bit = page_index % PAGES_PER_ENTRY;
            bitmap[entry] |= (1 << bit);
        }

        /// @brief Sets a page as free in the bitmap
        void set_page_free(uint64_t page_index) {
            uint64_t entry = page_index / PAGES_PER_ENTRY;
            uint64_t bit = page_index % PAGES_PER_ENTRY;
            bitmap[entry] &= ~(1 << bit);
        }

        /// @brief Checks if a page is allocated
        [[nodiscard]] bool is_page_allocated(uint64_t page_index) {
            uint64_t entry = page_index / PAGES_PER_ENTRY;
            uint64_t bit = page_index % PAGES_PER_ENTRY;
            return (bitmap[entry] & (1 << bit)) != 0;
        }

        /// @brief Finds the first free page in the bitmap
        [[nodiscard]] uint64_t find_free_page() {
            for (uint64_t i = 0; i < total_pages; i++) {
                if (!is_page_allocated(i)) {
                    return i;
                }
            }
            return UINT64_MAX;  // No free page found
        }

        /// @brief Finds contiguous free pages
        [[nodiscard]] uint64_t find_free_pages(uint64_t count) {
            uint64_t consecutive = 0;
            
            for (uint64_t i = 0; i < total_pages; i++) {
                if (!is_page_allocated(i)) {
                    consecutive++;
                    if (consecutive == count) {
                        return i - count + 1;  // Return start of the block
                    }
                } else {
                    consecutive = 0;
                }
            }
            
            return UINT64_MAX;  // Not enough contiguous pages
        }
    }

    bool initialize(uint64_t memory_map_addr, uint64_t memory_map_size) {
        (void)memory_map_addr;
        (void)memory_map_size;

        // Get memory map from Limine bootloader
        if (memmap_request.response == nullptr) {
            kernel::panic("Failed to get memory map from bootloader");
            return false;
        }

        limine_memmap_response* memmap = memmap_request.response;

        if (memmap->entry_count == 0) {
            kernel::panic("No memory map entries from bootloader");
            return false;
        }

        // Calculate total usable memory
        uint64_t usable_memory = 0;

        for (uint64_t i = 0; i < memmap->entry_count; i++) {
            limine_memmap_entry* entry = memmap->entries[i];

            // Only count usable memory
            if (entry->type == LIMINE_MEMMAP_USABLE) {
                usable_memory += entry->length;
            }
        }

        // Calculate total number of pages
        total_pages = usable_memory / PAGE_SIZE;

        // Ensure we don't exceed our maximum capacity
        if (total_pages > MAX_PAGES) {
            total_pages = MAX_PAGES;
        }

        // Initialize bitmap - mark all pages as free initially
        mem::set(bitmap, 0, sizeof(bitmap));

        // Mark reserved and bad regions as allocated to prevent usage
        for (uint64_t i = 0; i < memmap->entry_count; i++) {
            limine_memmap_entry* entry = memmap->entries[i];

            // Mark non-usable regions as allocated
            if (entry->type != LIMINE_MEMMAP_USABLE) {
                uint64_t start_page = entry->base / PAGE_SIZE;
                uint64_t end_page = (entry->base + entry->length) / PAGE_SIZE;

                for (uint64_t page = start_page; page < end_page && page < total_pages; page++) {
                    set_page_allocated(page);
                }
            }
        }

        allocated_pages = 0;
        for (uint64_t i = 0; i < total_pages; i++) {
            if (is_page_allocated(i)) {
                allocated_pages++;
            }
        }

        return true;
    }

    uint64_t allocate_page() {
        uint64_t page_index = find_free_page();

        if (page_index == UINT64_MAX) {
            return 0;  // Allocation failed
        }

        set_page_allocated(page_index);
        allocated_pages++;

        return page_index * PAGE_SIZE;
    }

    uint64_t allocate_pages(uint64_t page_count) {
        if (page_count == 0) {
            return 0;
        }

        uint64_t start_page = find_free_pages(page_count);

        if (start_page == UINT64_MAX) {
            return 0;  // Allocation failed
        }

        for (uint64_t i = 0; i < page_count; i++) {
            set_page_allocated(start_page + i);
            allocated_pages++;
        }

        return start_page * PAGE_SIZE;
    }

    void free_page(uint64_t page_addr) {
        if (page_addr == 0 || page_addr % PAGE_SIZE != 0) {
            return;
        }

        uint64_t page_index = page_addr / PAGE_SIZE;

        if (page_index >= total_pages) {
            return;
        }

        if (is_page_allocated(page_index)) {
            set_page_free(page_index);
            allocated_pages--;
        }
    }

    void free_pages(uint64_t page_addr, uint64_t page_count) {
        if (page_addr == 0 || page_addr % PAGE_SIZE != 0 || page_count == 0) {
            return;
        }

        uint64_t start_page = page_addr / PAGE_SIZE;

        for (uint64_t i = 0; i < page_count; i++) {
            uint64_t page_index = start_page + i;

            if (page_index >= total_pages) {
                break;
            }

            if (is_page_allocated(page_index)) {
                set_page_free(page_index);
                allocated_pages--;
            }
        }
    }

    uint64_t get_total_memory() {
        return total_pages * PAGE_SIZE;
    }

    uint64_t get_free_memory() {
        return (total_pages - allocated_pages) * PAGE_SIZE;
    }

    uint64_t get_allocated_memory() {
        return allocated_pages * PAGE_SIZE;
    }

}
