#pragma once

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>

namespace dh
{
    class binarizer
    {
    public:
        static void binarize( const cv::Mat& src, cv::Mat& dst, float threshold );
    };
}
