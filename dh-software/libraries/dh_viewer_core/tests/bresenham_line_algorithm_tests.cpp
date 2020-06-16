#include "bresenham_line_algorithm.h"

#include <gtest/gtest.h>

using namespace dh;
using namespace testing;

TEST( bresenham_line_algorithm_tests, works )
{
    QPoint p1( 0, 2 );
    QPoint p2( 9, 5 );

    auto actual_points = bresenham_line_algorithm::calculate( { p1, p2 } );

    QList<QPoint> expected_points =
    {
        { 0, 2 },
        { 1, 2 },
        { 2, 3 },
        { 3, 3 },
        { 4, 3 },
        { 5, 4 },
        { 6, 4 },
        { 7, 4 },
        { 8, 5 },
        { 9, 5 },
    };

    EXPECT_EQ( expected_points, actual_points );
}
