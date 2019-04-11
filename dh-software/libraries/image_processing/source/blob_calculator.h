#pragma once

#include "blob.h"

#include <opencv2/core.hpp>

#include <memory.h>

namespace dh
{
    class blob_calculator
    {
    public:
        static std::unique_ptr<blob[]> calculate( const cv::Mat&, size_t blobs_count );
    };
}
