#include "binarizer.h"
#include "image_processing.h"

#include <gtest/gtest.h>

using namespace dh;
using cv::Mat;

using namespace testing;

TEST( binarizer_tests, works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 1, 2, 10, 99, 100, 101, 199, 200 };

    auto cols = 8;
    auto rows = 1;

    auto image_8u = Mat( rows, cols, CV_8UC1, static_cast<void*>( data ) );
    auto bin_8u = Mat( rows, cols, CV_8UC1 );

    binarizer::binarize( image_8u, bin_8u, 0.5f );

    uint8_t expected_data[] = { 0, 0, 0, 0, 100, 101, 199, 200 };
    auto expected_bin_8u = Mat( rows, cols, CV_8UC1, static_cast<void*>( expected_data ) );

    auto diff = expected_bin_8u != bin_8u;
    bool eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( eq );
}
