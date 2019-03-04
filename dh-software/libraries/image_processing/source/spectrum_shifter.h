#pragma once

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>
#include <memory>

namespace dh
{
    class spectrum_shifter
    {
    public:
        spectrum_shifter( int step, int height, int channels );

        void shift( cv::Mat& m );

    private:
        std::unique_ptr<uint8_t[]> _buffer;
    };
}
