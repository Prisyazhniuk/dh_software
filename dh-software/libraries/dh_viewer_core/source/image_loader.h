#pragma once

#include <opencv2/core.hpp>

#include <QString>

namespace dh
{
    class image_loader
    {
    public:
        static cv::Mat load( const QString& path );
    };
}
