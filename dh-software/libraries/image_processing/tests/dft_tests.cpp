#include "dft.h"
#include "image_processing.h"
#include "image_converter.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <iostream>

#include <gtest/gtest.h>

using cv::Mat;
using cv::Scalar;

using namespace std;
using namespace testing;

TEST( dft_tests, constructor_wrong_channels_throws_exception )
{
    EXPECT_THROW( dft( 7, 7, 3 ), argument_exception );
}

TEST( dft_tests, forward_wrong_src_throws_exception )
{
    {
        dft dft( 7, 7, 1 );
        auto magnitudes_32f = Mat( 7, 7, CV_32FC1 );

        {
            auto src = Mat( 7, 8, CV_32FC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 8, 7, CV_32FC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 7, 7, CV_32FC3 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 7, 7, CV_8UC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }
    }
}

TEST( dft_tests, forward_wrong_magnitudes_throws_exception )
{
    {
        dft dft( 7, 7, 1 );
        auto src = Mat( 7, 7, CV_32FC1 );

        {
            auto magnitudes_32f = Mat( 7, 8, CV_32FC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto magnitudes_32f = Mat( 8, 7, CV_32FC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto magnitudes_32f = Mat( 7, 7, CV_32FC3 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto magnitudes_32f = Mat( 7, 7, CV_8UC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }
    }
}

TEST( dft_tests, works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0 };

    auto cols = 7;
    auto rows = 7;

    auto image_8u = Mat( rows, cols, CV_8UC1, static_cast<void*>( data ) );
    auto image_32f = Mat( rows, cols, CV_32FC1 );
    auto magnitudes_32f = Mat( rows, cols, CV_32FC1 );

    dft dft( cols, rows, image_8u.channels() );

    image_converter::convert_8u_32f( image_8u, image_32f );

    dft.forward( image_32f, magnitudes_32f );

    float expected_data[] = { 2295.00000f, 1718.93930f, 424.542940f, 613.482360f, 613.482360f, 424.542940f, 1718.93930f,
                              1718.93930f, 1287.47390f, 317.979800f, 459.494170f, 459.494110f, 317.979800f, 1287.47390f,
                               424.54294f,  317.97980f,  78.534523f, 113.485680f, 113.485695f,  78.534523f,  317.97977f,
                               613.48236f,  459.49411f, 113.485680f, 163.991560f, 163.991560f, 113.485680f,  459.49411f,
                               613.48236f,  459.49411f, 113.485680f, 163.991560f, 163.991560f, 113.485680f,  459.49411f,
                               424.54294f,  317.97977f,  78.534523f, 113.485695f, 113.485680f,  78.534523f,  317.97980f,
                              1718.93930f, 1287.47390f, 317.979800f, 459.494110f, 459.494170f, 317.979800f, 1287.47390f };

    auto expected_magnitudes_32f = Mat( rows, cols, CV_32FC1, static_cast<void*>( expected_data ) );

    EXPECT_EQ( expected_magnitudes_32f.rows, magnitudes_32f.rows );
    EXPECT_EQ( expected_magnitudes_32f.cols, magnitudes_32f.cols );
    EXPECT_EQ( expected_magnitudes_32f.channels(), magnitudes_32f.channels() );

    auto diff = expected_magnitudes_32f != magnitudes_32f;
    bool magnitudes_are_eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( magnitudes_are_eq );
}
