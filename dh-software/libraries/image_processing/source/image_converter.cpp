#include "image_converter.h"
#include "dh_string.h"

#include <ippi.h>
#include <ipps.h>
#include <ippcore.h>

using namespace cv;
using namespace std;

namespace dh
{
    void image_converter::convert_8u_32f( const Mat& image_8u, Mat& image_32f, IppiSize roi )
    {
        if( roi.width < 0 )
            roi.width = image_8u.cols;

        if( roi.height < 0 )
            roi.height = image_8u.rows;

        if( roi.width > image_8u.cols )
            throw argument_exception( dh_string::fs( "roi width too large: %d, image_8u width: %d",
                                                     roi.width, image_8u.cols ),
                                      get_exception_source() );

        if( roi.height > image_8u.rows )
            throw argument_exception( dh_string::fs( "roi height too large: %d, image_8u height: %d",
                                                     roi.height, image_8u.rows ),
                                      get_exception_source() );

        if( roi.width > image_32f.cols )
            throw argument_exception( dh_string::fs( "roi width too large: %d, image_32f width: %d",
                                                     roi.width, image_32f.cols ),
                                      get_exception_source() );

        if( roi.height > image_32f.rows )
            throw argument_exception( dh_string::fs( "roi height too large: %d, image_32f height: %d",
                                                     roi.height, image_32f.rows ),
                                      get_exception_source() );


        if( image_8u.channels() != image_32f.channels() )
            throw argument_exception( "input images has different channels count", get_exception_source() );

        if( image_8u.depth() != CV_8U )
            throw argument_exception( "image_8u has wrong depth", get_exception_source() );

        if( image_32f.depth() != CV_32F )
            throw argument_exception( "image_32f has wrong depth", get_exception_source() );

        IppStatus status = ippStsNoErr;

        if( image_8u.channels() == 1 )
        {
            status = ippiConvert_8u32f_C1R( image_8u.data, static_cast<int>( image_8u.step ),
                                            image_32f.ptr<float>(), static_cast<int>( image_32f.step ), roi );
        }
        else if( image_8u.channels() == 3 )
        {
            status = ippiConvert_8u32f_C3R( image_8u.data, static_cast<int>( image_8u.step ),
                                            image_32f.ptr<float>(), static_cast<int>( image_32f.step ), roi );
        }
        else
        {
            throw argument_exception( "unsupported channels count", get_exception_source() );
        }

        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
    }

    void image_converter::convert_32f_8u( const Mat& image_32f, Mat& image_8u, IppiSize roi )
    {
        if( roi.width < 0 )
            roi.width = image_32f.cols;

        if( roi.height < 0 )
            roi.height = image_32f.rows;

        if( roi.width > image_32f.cols )
            throw argument_exception( dh_string::fs( "roi width too large: %d, image_32f width: %d",
                                                     roi.width, image_32f.cols ),
                                      get_exception_source() );

        if( roi.height > image_32f.rows )
            throw argument_exception( dh_string::fs( "roi height too large: %d, image_32f height: %d",
                                                     roi.height, image_32f.rows ),
                                      get_exception_source() );

        if( roi.width > image_8u.cols )
            throw argument_exception( dh_string::fs( "roi width too large: %d, image_8u width: %d",
                                                     roi.width, image_8u.cols ),
                                      get_exception_source() );

        if( roi.height > image_8u.rows )
            throw argument_exception( dh_string::fs( "roi height too large: %d, image_8u height: %d",
                                                     roi.height, image_8u.rows ),
                                      get_exception_source() );

        if( image_8u.channels() != image_32f.channels() )
            throw argument_exception( "input images has different channels count", get_exception_source() );

        if( image_8u.depth() != CV_8U )
            throw argument_exception( "image_8u has wrong depth", get_exception_source() );

        if( image_32f.depth() != CV_32F )
            throw argument_exception( "image_32f has wrong depth", get_exception_source() );

        IppStatus status = ippStsNoErr;

        if( image_32f.channels() == 1 )
        {
            status = ippiConvert_32f8u_C1R( image_32f.ptr<float>(), static_cast<int>( image_32f.step ),
                                            image_8u.data, static_cast<int>( image_8u.step ), roi, ippRndNear );
        }
        else if( image_32f.channels() == 3 )
        {
            status = ippiConvert_32f8u_C3R( image_32f.ptr<float>(), static_cast<int>( image_32f.step ),
                                            image_8u.data, static_cast<int>( image_8u.step ), roi, ippRndNear );
        }
        else
        {
            throw argument_exception( "unsupported channels count", get_exception_source() );
        }

        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
    }

    void image_converter::convert_8u_32fc( const Mat& image_8u, image_32fc& image_32fc )
    {
        if( image_8u.channels() != 1 )
            throw argument_exception( "only one-channel images supported", get_exception_source() );

        if( image_8u.depth() != CV_8U )
            throw argument_exception( "image_8u has wrong depth", get_exception_source() );

        if( image_8u.cols != image_32fc.width() )
            throw argument_exception( "images width is different", get_exception_source() );

        if( image_8u.rows != image_32fc.height() )
            throw argument_exception( "images height is different", get_exception_source() );

        for( int r = 0; r < image_8u.rows; r++ )
            for( int c = 0; c < image_8u.cols; c++ )
                image_32fc.at(c, r) = { float( image_8u.at<uint8_t>(r, c) ), 0.0f };
    }

    void image_converter::separate_32fc( const image_32fc& image_32fc, Mat& real_32f, Mat& imaginary_32f )
    {
        if( real_32f.channels() != 1 || imaginary_32f.channels() != 1 )
            throw argument_exception( "only one-channel images supported", get_exception_source() );

        if( real_32f.depth() != CV_32F || imaginary_32f.depth() != CV_32F )
            throw argument_exception( "invalid depth", get_exception_source() );

        auto width = image_32fc.width();
        auto height = image_32fc.height();

        if( real_32f.cols != width || imaginary_32f.cols != width )
            throw argument_exception( "images width is different", get_exception_source() );

        if( real_32f.rows != height || imaginary_32f.rows != height )
            throw argument_exception( "images height is different", get_exception_source() );

        for( auto r = 0; r < image_32fc.height(); r++ )
        {
            auto status = ippsCplxToReal_32fc( &image_32fc.at( 0, r ),
                                               &real_32f.at<float>( r, 0 ),
                                               &imaginary_32f.at<float>( r, 0 ),
                                               width );
            if( status != ippStsNoErr )
                throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
        }
    }
}
