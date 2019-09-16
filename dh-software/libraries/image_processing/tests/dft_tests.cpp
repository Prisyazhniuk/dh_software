#include "dft.h"
#include "image_processing.h"
#include "image_converter.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <ippi.h>

#include <gtest/gtest.h>

using namespace dh;
using cv::Mat;

using namespace testing;

TEST( dft_tests, forward_wrong_src_throws_exception )
{
    {
        dft dft( 7, 7 );
        auto dst = image_32fc( 7, 7 );

        {
            auto src = image_32fc( 7, 8 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }

        {
            auto src = image_32fc( 8, 7 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }
    }
}

TEST( dft_tests, forward_wrong_dst_throws_exception )
{
    {
        dft dft( 7, 7 );
        auto src = image_32fc( 7, 7 );

        {
            auto dst = image_32fc( 7, 8 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }

        {
            auto dst = image_32fc( 8, 7 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }
    }
}

TEST( dft_tests, forward_works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0 };

    auto width = 7;
    auto height = 7;

    auto src_32fc = image_32fc( width, height );
    auto dst_32fc = image_32fc( width, height );

    auto src_8u = Mat( height, width, CV_8UC1, static_cast<void*>( data ) );

    image_converter::convert_8u_32fc( src_8u, src_32fc );

    dft dft( width, height );

    dft.forward( src_32fc, dst_32fc );

    auto magnitudes_32f = Mat( height, width, CV_32FC1 );

    auto status = ippiMagnitude_32fc32f_C1R( dst_32fc.data(), dst_32fc.step_in_bytes(),
                                             magnitudes_32f.ptr<float>(), int( magnitudes_32f.step ),
                                             { width, height } );
    if( status != ippStsNoErr )
        throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );

    float expected_data[] = { 2295.00000f, 1718.93930f, 424.542940f, 613.482360f, 613.482360f, 424.542940f, 1718.93930f,
                              1718.93930f, 1287.47390f, 317.979800f, 459.494170f, 459.494110f, 317.979800f, 1287.47390f,
                               424.54294f,  317.97980f,  78.534523f, 113.485680f, 113.485695f,  78.534523f,  317.97977f,
                               613.48236f,  459.49411f, 113.485680f, 163.991560f, 163.991560f, 113.485680f,  459.49411f,
                               613.48236f,  459.49411f, 113.485680f, 163.991560f, 163.991560f, 113.485680f,  459.49411f,
                               424.54294f,  317.97977f,  78.534523f, 113.485695f, 113.485680f,  78.534523f,  317.97980f,
                              1718.93930f, 1287.47390f, 317.979800f, 459.494110f, 459.494170f, 317.979800f, 1287.47390f };

    auto expected_magnitudes_32f = Mat( height, width, CV_32FC1, static_cast<void*>( expected_data ) );

    auto diff = expected_magnitudes_32f != magnitudes_32f;
    bool magnitudes_are_eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( magnitudes_are_eq );
}


TEST( dft_tests, inverse_wrong_src_throws_exception )
{
    {
        dft dft( 7, 7 );
        auto dst = image_32fc( 7, 7 );

        {
            auto src = image_32fc( 7, 8 );
            EXPECT_THROW( dft.inverse( src, dst ), argument_exception );
        }

        {
            auto src = image_32fc( 8, 7 );
            EXPECT_THROW( dft.inverse( src, dst ), argument_exception );
        }
    }
}

TEST( dft_tests, inverse_wrong_dst_throws_exception )
{
    {
        dft dft( 7, 7 );
        auto src = image_32fc( 7, 7 );

        {
            auto dst = image_32fc( 7, 8 );
            EXPECT_THROW( dft.inverse( src, dst ), argument_exception );
        }

        {
            auto dst = image_32fc( 8, 7 );
            EXPECT_THROW( dft.inverse( src, dst ), argument_exception );
        }
    }
}

TEST( dft_tests, inverse_works )
{
    image_processing::initialize_ipp();

    const Ipp32fc data[] = {
        {  2295.00000000f,     0.00000000f }, { -1548.71088237f,  -745.81985282f }, {   264.69821290f,   331.92106042f }, {   136.51266947f,   598.10108409f }, {   136.51266947f,  -598.10108409f }, {   264.69821290f,  -331.92106042f }, { -1548.71088237f,   745.81985282f },
        { -1548.71088237f,  -745.81985282f }, {   802.72686026f,  1006.58764475f }, {   -70.75716184f,  -310.00738150f }, {   102.24706132f,  -447.97364564f }, {  -286.48989947f,   359.24697108f }, {  -286.48989947f,   137.96626413f }, {  1287.47392158f,     0.00000000f },
        {   264.69821290f,   331.92106042f }, {   -70.75716184f,  -310.00738150f }, {   -17.47557579f,    76.56550023f }, {   -70.75716184f,    88.72667455f }, {   102.24706132f,   -49.23958958f }, {    78.53452473f,    -0.00000000f }, {  -286.48989947f,  -137.96626413f },
        {   136.51266947f,   598.10108409f }, {   102.24706132f,  -447.97364564f }, {   -70.75716184f,    88.72667455f }, {  -147.75128447f,    71.15326850f }, {   163.99155369f,     0.00000000f }, {   102.24706132f,    49.23958958f }, {  -286.48989947f,  -359.24697108f },
        {   136.51266947f,  -598.10108409f }, {  -286.48989947f,   359.24697108f }, {   102.24706132f,   -49.23958958f }, {   163.99155369f,     0.00000000f }, {  -147.75128447f,   -71.15326850f }, {   -70.75716184f,   -88.72667455f }, {   102.24706132f,   447.97364564f },
        {   264.69821290f,  -331.92106042f }, {  -286.48989947f,   137.96626413f }, {    78.53452473f,     0.00000000f }, {   102.24706132f,    49.23958958f }, {   -70.75716184f,   -88.72667455f }, {   -17.47557579f,   -76.56550023f }, {   -70.75716184f,   310.00738150f },
        { -1548.71088237f,   745.81985282f }, {  1287.47392158f,    -0.00000000f }, {  -286.48989947f,  -137.96626413f }, {  -286.48989947f,  -359.24697108f }, {   102.24706132f,   447.97364564f }, {   -70.75716184f,   310.00738150f }, {   802.72686026f, -1006.58764475f }
    };

    auto width = 7;
    auto height = 7;

    auto src_32fc = image_32fc( width, height, data, 7*sizeof(Ipp32fc) );
    auto dst_32fc = image_32fc( width, height );

    dft dft( width, height );

    dft.inverse( src_32fc, dst_32fc );

    auto magnitudes_32f = Mat( height, width, CV_32FC1 );

    auto status = ippiMagnitude_32fc32f_C1R( dst_32fc.data(), dst_32fc.step_in_bytes(),
                                             magnitudes_32f.ptr<float>(), int( magnitudes_32f.step ),
                                             { width, height } );
    if( status != ippStsNoErr )
        throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );

    float expected_data[] = { 0.000005f, 0.000007f,   0.000009f,   0.000009f,   0.000009f, 0.000001f, 0.000002f,
                              0.000003f, 0.000007f,   0.000004f,   0.000009f,   0.000009f, 0.000001f, 0.000003f,
                              0.000001f, 0.000002f, 255.000031f, 255.000031f, 255.000031f, 0.000002f, 0.000002f,
                              0.000004f, 0.000006f, 255.000031f, 255.000031f, 255.000031f, 0.000002f, 0.000004f,
                              0.000004f, 0.000008f, 255.000031f, 255.000031f, 255.000031f, 0.000003f, 0.000001f,
                              0.000001f, 0.000001f,   0.000009f,   0.000009f,   0.000009f, 0.000002f, 0.000001f,
                              0.000003f, 0.000003f,   0.000004f,   0.000009f,   0.000009f, 0.000002f, 0.000002f };

    for( int r = 0; r < height; r++ )
        for( int c = 0; c < width; c++ )
            EXPECT_NEAR( double( expected_data[width*r+c] ),
                         double( magnitudes_32f.at<float>(r,c) ),
                         0.00001 );
}
