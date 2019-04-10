#pragma once

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>
#include <ippcore.h>

namespace dh
{
    class gauss_filter
    {
    public:
        gauss_filter( int width, int height, int channels, float sigma );
        ~gauss_filter();

        void filter( const cv::Mat& src, cv::Mat& dst );

    private:
        IppFilterGaussianSpec* _context;
        uint8_t* _buffer;
    };
}
