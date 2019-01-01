#include "image_converter.h"

#include <ippi.h>
#include <ippcore.h>

#include <string>

using namespace cv;
using namespace std;

void image_converter::convert_8u_32f( const Mat& image_8u, Mat& image_32f )
{
	if( image_8u.rows != image_32f.rows ||
		image_8u.cols != image_32f.cols ||
		image_8u.channels()!= image_32f.channels() )
	{
		throw argument_exception( "input images has different size", get_exception_source() );
	}

	if( image_8u.type() != CV_8U )
		throw argument_exception( "image_8u has wrong format", get_exception_source() );

	if( image_32f.type() != CV_32F )
		throw argument_exception( "image_32f has wrong format", get_exception_source() );

	IppiSize roi = { image_8u.cols, image_8u.rows };

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

void image_converter::convert_32f_8u( const Mat& image_32f, Mat& image_8u )
{
	if( image_32f.rows != image_8u.rows ||
		image_32f.cols != image_8u.cols ||
		image_32f.channels()!= image_8u.channels() )
	{
		throw argument_exception( "input images has different size", get_exception_source() );
	}

	if( image_8u.type() != CV_8U )
		throw argument_exception( "image_8u has wrong format", get_exception_source() );

	if( image_32f.type() != CV_32F )
		throw argument_exception( "image_32f has wrong format", get_exception_source() );


	IppiSize roi = { image_32f.cols, image_32f.rows };

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
