#pragma once

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>
#include <ippcore.h>

class fft
{
public:
    fft( int width, int height, int channels );
    ~fft();

	int get_width();
	int get_height();

    void forward( const cv::Mat& src, cv::Mat& dst );

private:
    const int _flags = IPP_FFT_DIV_INV_BY_N;
    const IppHintAlgorithm _hint = ippAlgHintNone;

	int _width;
	int _height;
    int _channels;

    IppiFFTSpec_R_32f* _context;
	uint8_t* _init_buffer;
	uint8_t* _work_buffer;

    uint8_t* _repack_buffer;

    cv::Mat _coefficients;
};
