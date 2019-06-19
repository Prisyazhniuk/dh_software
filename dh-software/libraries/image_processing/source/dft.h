#pragma once

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>
#include <ippcore.h>

namespace dh
{
    class dft
    {
    public:
        dft( int width, int height, int channels );
        ~dft();

        void forward( const cv::Mat& src, cv::Mat& dst );

    private:
        const int _flags = IPP_FFT_NODIV_BY_ANY;
        const IppHintAlgorithm _hint = ippAlgHintNone;

        int _width;
        int _height;
        int _channels;

        IppiDFTSpec_R_32f* _context;
        uint8_t* _init_buffer;
        uint8_t* _work_buffer;

        uint8_t* _repack_buffer;

        cv::Mat _coefficients;
    };
}
