#include "fft.h"
#include "image_processing.h"
#include "image_converter.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <iostream>

#include <gtest/gtest.h>

using namespace cv;
using namespace std;
using namespace testing;

TEST( fft_tests, works )
{
    try
    {
        image_processing::initialize_ipp();

        auto image_8u = imread( "input.bmp", IMREAD_GRAYSCALE );
        if( image_8u.empty() )
            throw dh_exception( "file open error", get_exception_source() );

        IppiSize roi = { image_8u.cols, image_8u.rows };

        fft fft( image_8u.cols, image_8u.rows, image_8u.channels() );

        auto cols = fft.get_width();
        auto rows = fft.get_height();

        auto image_32f = Mat( rows, cols, CV_32FC1, Scalar( 0 ) );
        auto magnitudes_32f = Mat( rows, cols, CV_32FC1 );

        image_converter::convert_8u_32f( image_8u, image_32f, roi );

        fft.forward( image_32f, magnitudes_32f );

        // cout << magnitudes_32f << endl;

        double min, max;
        minMaxLoc( magnitudes_32f, &min, &max );

        for( int r = 0; r < magnitudes_32f.rows; r++ )
            for( int c = 0; c < magnitudes_32f.cols; c++ )
            {
                auto p = magnitudes_32f.at<float>( r, c );
                p = 255 * ( p - min ) / ( max - min );
                magnitudes_32f.at<float>( r, c ) = p;
            }

        auto result_8u = Mat( magnitudes_32f.rows, magnitudes_32f.cols, CV_8UC1 );

        image_converter::convert_32f_8u( magnitudes_32f, result_8u );

        imwrite( "forward_fft.bmp", result_8u );
    }
    catch( dh_exception& ex )
    {
        cout << ex.c_str() << endl;
    }
    catch( ... )
    {
        cout << "unhundled exception" << endl;
    }
}
