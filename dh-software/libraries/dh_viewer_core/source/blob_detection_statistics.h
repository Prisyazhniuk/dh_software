#pragma once

#include "blob.h"

#include <cstdint>

namespace dh
{
    struct blob_detection_statistics
    {
        static const size_t max_blobs_count = 2;

        uint64_t time_us;

        size_t blobs_count;
        blob blobs[max_blobs_count];
    };
}
