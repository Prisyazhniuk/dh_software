#pragma once

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <opencv2/core.hpp>

class image_converter
{
public:
	static void convert_8u_32f( const cv::Mat& image_8u, cv::Mat& image_32f );
	static void convert_32f_8u( const cv::Mat& image_32f, cv::Mat& image_8u );
};

