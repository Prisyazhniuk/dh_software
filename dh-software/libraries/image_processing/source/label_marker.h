#pragma once

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>

namespace dh
{
    class label_marker
    {
    public:
        label_marker( int width, int height, int channels );
        ~label_marker();

        int mark( const cv::Mat& );

    private:
        uint8_t* _buffer;
    };
}
