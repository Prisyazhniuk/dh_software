#include "image_loader.h"

#include <opencv2/imgcodecs.hpp>

using namespace cv;

namespace dh
{
    Mat image_loader::load( const QString& path )
    {
        auto image = imread( path.toLocal8Bit().toStdString(), IMREAD_GRAYSCALE );

        if( image.empty() )
            throw QString( "Ошибка загрузки изображения" );

        return image;
    }
}
