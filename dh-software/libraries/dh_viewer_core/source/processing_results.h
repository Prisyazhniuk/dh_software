#pragma once

#include <opencv2/core.hpp>

#include <optional>

namespace dh
{
    struct processing_results
    {
        std::optional<cv::Mat> original;
        std::optional<cv::Mat> phase;
        std::optional<cv::Mat> amplitude;
        std::optional<cv::Mat> intensity;
        std::optional<cv::Mat> real;
        std::optional<cv::Mat> imaginary;
    };
}
