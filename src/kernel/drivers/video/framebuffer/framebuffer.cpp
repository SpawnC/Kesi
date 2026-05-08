#include "framebuffer.hpp"
#include <limine.h>
#include <stdint.h>
#include "../../../graphics/colors/framebuffer/framebuffer_colors.hpp"

namespace drivers::video::framebuffer {


    namespace {

        __attribute__((used, section(".requests")))
        volatile limine_framebuffer_request framebuffer_request = {
            LIMINE_FRAMEBUFFER_REQUEST_ID,
            0,
            nullptr
        };

        limine_framebuffer* g_framebuffer = nullptr;
    }

    uint64_t width  = 0;
    uint64_t height = 0;
    uint32_t* address = nullptr;
    uint64_t pitch = 0;   

    bool initialize() {

        if (framebuffer_request.response == nullptr)
            return false;

        if (framebuffer_request.response->framebuffer_count < 1)
            return false;

        g_framebuffer =
            framebuffer_request.response->framebuffers[0];

        address =
            static_cast<uint32_t*>(g_framebuffer->address);

        width  = g_framebuffer->width;
        height = g_framebuffer->height;
        pitch  = g_framebuffer->pitch;

        return true;
    }

    void put_pixel(uint32_t x,
                   uint32_t y,
                   uint32_t color) {

        if (x >= width || y >= height)
            return;

        uint8_t* row =
            reinterpret_cast<uint8_t*>(address) + y * pitch;

        reinterpret_cast<uint32_t*>(row)[x] = color;
    }

    void clear(graphics::colors::framebuffer::Color color) {

        for (uint64_t y = 0; y < height; y++) {

            for (uint64_t x = 0; x < width; x++) {

                put_pixel(x, y, static_cast<uint32_t>(color));
            }
        }
    }

}