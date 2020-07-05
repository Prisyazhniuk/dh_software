#pragma once

#include "image_32fc.h"

#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>

namespace dh
{
    class image_processing
    {
    public:
        static void initialize_ipp();

        static uint8_t max_8u( const cv::Mat& image_8u );
        static void min_max_8u( const cv::Mat& image_8u, uint8_t& min, uint8_t& max );
        static uint8_t mean_8u( const cv::Mat& image_8u );

        static void min_max_32f( const cv::Mat& image_32f, float& min, float& max );
        static float mean_32f( const cv::Mat& image_32f );
    };
}
