#pragma once

#include <cstdint>

namespace dh
{
    struct fft_processing_statistics
    {
        uint64_t period_us;

        uint32_t frames_processed;
    };
}
