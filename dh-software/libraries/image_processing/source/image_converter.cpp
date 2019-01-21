#include "image_converter.h"
#include "dh_string.h"

#include <ippi.h>
#include <ippcore.h>

using namespace cv;
using namespace std;

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
