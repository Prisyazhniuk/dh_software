#pragma once

#include "image_32fc.h"

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>

#include <ippcore.h>

namespace dh
{
    class image_converter
    {
    public:
        static void convert_8u_32f( const cv::Mat& image_8u, cv::Mat& image_32f, IppiSize roi = { -1, -1 } );
        static void convert_32f_8u( const cv::Mat& image_32f, cv::Mat& image_8u, IppiSize roi = { -1, -1 } );

        static void convert_8u_32fc( const cv::Mat& image_8u, image_32fc& image_32fc );
    };
}
