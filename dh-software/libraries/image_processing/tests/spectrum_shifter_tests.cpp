#include "spectrum_shifter.h"
#include "image_processing.h"

#include <gtest/gtest.h>

using namespace dh;
using namespace testing;

TEST( spectrum_shifter_tests, works_for_even )
{
    image_processing::initialize_ipp();

    const Ipp32fc data[] =
    {
        { 1,  101 }, { 2,  102 }, { 3,  103 }, { 4,  104 }, { 5,  105 }, { 6,  106 },
        { 7,  107 }, { 8,  108 }, { 9,  109 }, { 10, 110 }, { 11, 111 }, { 12, 112 },
        { 13, 113 }, { 14, 114 }, { 15, 115 }, { 16, 116 }, { 17, 117 }, { 18, 118 },
        { 19, 119 }, { 20, 120 }, { 21, 121 }, { 22, 122 }, { 23, 123 }, { 24, 124 }
    };

    const Ipp32fc expected_data[] =
    {
        { 16, 116 }, { 17, 117 }, { 18, 118 }, { 13, 113 }, { 14, 114 }, { 15, 115 },
        { 22, 122 }, { 23, 123 }, { 24, 124 }, { 19, 119 }, { 20, 120 }, { 21, 121 },
        { 4,  104 }, { 5,  105 }, { 6,  106 }, { 1,  101 }, { 2,  102 }, { 3,  103 },
        { 10, 110 }, { 11, 111 }, { 12, 112 }, { 7,  107 }, { 8,  108 }, { 9,  109 }
    };

    int step = 6 * sizeof(Ipp32fc);
    auto actual_image = image_32fc( 6, 4, data, step );
    auto expected_image = image_32fc( 6, 4, expected_data, step );

    spectrum_shifter shifter( actual_image.width(), actual_image.height() );
    shifter.shift( actual_image );

    for( int y = 0; y < actual_image.height(); y++ )
    {
        for( int x = 0; x < actual_image.width(); x++ )
        {
            EXPECT_EQ( expected_image.at(x, y).im, actual_image.at(x, y).im );
            EXPECT_EQ( expected_image.at(x, y).re, actual_image.at(x, y).re );
        }
    }
}

TEST( spectrum_shifter_tests, works_for_odd )
{
    image_processing::initialize_ipp();

    const Ipp32fc data[] =
    {
        { 1,  101 }, { 2,  102 }, { 3,  103 }, { 4,  104 }, { 5,  105 },
        { 6,  106 }, { 7,  107 }, { 8,  108 }, { 9,  109 }, { 10, 110 },
        { 11, 111 }, { 12, 112 }, { 13, 113 }, { 14, 114 }, { 15, 115 },
        { 16, 116 }, { 17, 117 }, { 18, 118 }, { 19, 119 }, { 20, 120 },
        { 21, 121 }, { 22, 122 }, { 23, 123 }, { 24, 124 }, { 25, 125 }
    };

    const Ipp32fc expected_data[] =
    {
        { 19, 119 }, { 20, 120 }, { 16, 116 }, { 17, 117 }, { 18, 118 },
        { 24, 124 }, { 25, 125 }, { 21, 121 }, { 22, 122 }, { 23, 123 },
        { 4,  104 }, { 5,  105 }, { 1,  101 }, { 2,  102 }, { 3,  103 },
        { 9,  109 }, { 10, 110 }, { 6,  106 }, { 7,  107 }, { 8,  108 },
        { 14, 114 }, { 15, 115 }, { 11, 111 }, { 12, 112 }, { 13, 113 }
    };

    int step = 5 * sizeof(Ipp32fc);
    auto actual_image = image_32fc( 5, 5, data, step );
    auto expected_image = image_32fc( 5, 5, expected_data, step );

    spectrum_shifter shifter( actual_image.width(), actual_image.height() );
    shifter.shift( actual_image );

    for( int y = 0; y < actual_image.height(); y++ )
    {
        for( int x = 0; x < actual_image.width(); x++ )
        {
            EXPECT_EQ( expected_image.at(x, y).im, actual_image.at(x, y).im );
            EXPECT_EQ( expected_image.at(x, y).re, actual_image.at(x, y).re );
        }
    }
}
