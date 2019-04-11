#include "label_marker.h"
#include "image_processing.h"

#include <gtest/gtest.h>

using namespace dh;
using cv::Mat;

using namespace testing;

TEST( label_marker_tests, works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 0, 0, 0, 0, 0, 0, 0, 0,
                       4, 5, 0, 0, 0, 1, 1, 1,
                       6, 7, 0, 0 ,0, 1, 1, 1,
                       0, 8, 9, 0, 0, 0, 0, 0,
                       0, 4, 5, 0, 7, 7, 7, 0,
                       0, 0, 0, 0, 7, 7, 7, 0,
                       0, 0, 0, 0, 7, 7, 7, 0,
                       0, 0, 0, 0, 0, 7, 0, 0 };

    uint8_t expected_data[] = { 0, 0, 0, 0, 0, 0, 0, 0,
                                1, 1, 0, 0, 0, 2, 2, 2,
                                1, 1, 0, 0 ,0, 2, 2, 2,
                                0, 1, 1, 0, 0, 0, 0, 0,
                                0, 1, 1, 0, 3, 3, 3, 0,
                                0, 0, 0, 0, 3, 3, 3, 0,
                                0, 0, 0, 0, 3, 3, 3, 0,
                                0, 0, 0, 0, 0, 3, 0, 0 };

    auto cols = 8;
    auto rows = 8;

    auto image_8u = Mat( rows, cols, CV_8UC1, static_cast<void*>( data ) );

    label_marker marker( cols, rows, 1 );
    auto count = marker.mark( image_8u );

    EXPECT_EQ( 3, count );

    auto expected_image_8u = Mat( rows, cols, CV_8UC1, static_cast<void*>( expected_data ) );

    auto diff = expected_image_8u != image_8u;
    bool eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( eq );
}
