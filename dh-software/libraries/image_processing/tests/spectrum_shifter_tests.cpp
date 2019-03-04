#include "spectrum_shifter.h"

#include <gtest/gtest.h>
#include <iostream>

using namespace dh;
using namespace cv;
using namespace testing;

TEST( spectrum_shifter_tests, works_for_even )
{
    uint8_t actual_data[] = {  1,  2,  3,  4,  5,  6,
                               7,  8,  9, 10, 11, 12,
                              13, 14, 15, 16, 17, 18,
                              19, 20, 21, 22, 23, 24 };

    uint8_t expected_data[] = { 16, 17, 18, 13, 14, 15,
                                22, 23, 24, 19, 20, 21,
                                 4,  5,  6,  1,  2,  3,
                                10, 11, 12,  7,  8,  9 };

    auto actual = Mat( 4, 6, CV_8UC1, static_cast<void*>( actual_data ) );
    auto expected = Mat( 4, 6, CV_8UC1, static_cast<void*>( expected_data ) );

    spectrum_shifter shifter( actual.cols, actual.rows, 1 );
    shifter.shift( actual );

    auto diff = expected != actual;
    bool matrix_are_eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( matrix_are_eq );
}

TEST( spectrum_shifter_tests, works_for_odd )
{
    uint8_t actual_data[] = {  1,  2,  3,  4,  5,
                               6,  7,  8,  9, 10,
                              11, 12, 13, 14, 15,
                              16, 17, 18, 19, 20,
                              21, 22, 23, 24, 25 };

    uint8_t expected_data[] = { 19, 20, 16, 17, 18,
                                24, 25, 21, 22, 23,
                                 4,  5,  1,  2,  3,
                                 9, 10,  6,  7,  8,
                                14, 15, 11, 12, 13 };

    auto actual = Mat( 5, 5, CV_8UC1, static_cast<void*>( actual_data ) );
    auto expected = Mat( 5, 5, CV_8UC1, static_cast<void*>( expected_data ) );

    spectrum_shifter shifter( actual.cols, actual.rows, 1 );
    shifter.shift( actual );

    auto diff = expected != actual;
    bool matrix_are_eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( matrix_are_eq );
}
