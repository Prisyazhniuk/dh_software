#include "blob_calculator.h"

using namespace std;

namespace dh
{
    unique_ptr<blob[]> blob_calculator::calculate( const cv::Mat& m, size_t blobs_count )
    {
        // TODO
        // check mat channel count, type

        auto blobs = make_unique<blob[]>( blobs_count );
        auto x = make_unique<uint32_t[]>( blobs_count );
        auto y = make_unique<uint32_t[]>( blobs_count );

        for( size_t i = 0; i < blobs_count; i++ )
        {
            blobs[i].area = 0;
            x[i] = 0;
            y[i] = 0;
        }

        for( int r = 0; r < m.rows; r++ )
            for( int c = 0; c < m.cols; c++ )
            {
                auto px = m.at<uint8_t>( r, c );
                if( px != 0 && px <= blobs_count )
                {
                    x[ px-1 ] += uint32_t( c );
                    y[ px-1 ] += uint32_t( r );
                    blobs[ px-1 ].area++;
                }
            }

        for( size_t i = 0; i < blobs_count; i++ )
        {
            blobs[i].center_x = blobs[i].area ? int( x[i] / blobs[i].area ) : 0;
            blobs[i].center_y = blobs[i].area ? int( y[i] / blobs[i].area ) : 0;
        }

        return blobs;
    }
}
