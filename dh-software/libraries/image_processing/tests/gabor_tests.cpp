#include "gabor.h"

#include "image_processing.h"

#include <gtest/gtest.h>

using namespace dh;
using namespace std;

using namespace testing;

TEST( gabor_tests, make_kernel_works )
{
    image_processing::initialize_ipp();

    auto lambda = 0.0006328f;
    auto distance = 23.2f;
    auto pixel_size = 4.24f / 1280;

    int width = 5;
    int height = 5;

    auto kernel = image_32fc( width, height );

    gabor::make_kernel( kernel, lambda, distance, pixel_size );

    const Ipp32fc expected_kernel[] = {
        { 18.886269f, 65.444794f }, { 19.039885f, 65.400269f }, { 18.886269f, 65.444794f }, { 18.424801f, 65.576202f }, { 17.653669f, 65.787994f },
        { 19.039885f, 65.400269f }, { 19.193394f, 65.355385f }, { 19.039885f, 65.400269f }, { 18.578728f, 65.532761f }, { 17.808094f, 65.746361f },
        { 18.886269f, 65.444794f }, { 19.039885f, 65.400269f }, { 18.886269f, 65.444794f }, { 18.424801f, 65.576202f }, { 17.653669f, 65.787994f },
        { 18.424801f, 65.576202f }, { 18.578728f, 65.532761f }, { 18.424801f, 65.576202f }, { 17.962420f, 65.704369f }, { 17.189816f, 65.910713f },
        { 17.653669f, 65.787994f }, { 17.808094f, 65.746361f }, { 17.653669f, 65.787994f }, { 17.189816f, 65.910713f }, { 16.414839f, 66.107979f },
    };

    int i = 0;
    for( auto r = 0; r < kernel.height(); r++ )
        for( auto c = 0; c < kernel.width(); c++, i++ )
        {
            EXPECT_FLOAT_EQ( expected_kernel[i].re, kernel.at(c,r).re );
            EXPECT_FLOAT_EQ( expected_kernel[i].im, kernel.at(c,r).im );
        }
}
