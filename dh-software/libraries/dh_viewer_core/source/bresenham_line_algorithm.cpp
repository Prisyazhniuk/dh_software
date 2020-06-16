#include "bresenham_line_algorithm.h"

#include <math.h>

using namespace std;

namespace dh
{
    QList<QPoint> bresenham_line_algorithm::calculate( const QLineF& line )
    {
        QList<QPoint> points;

        auto x1 = float( line.p1().x() );
        auto y1 = float( line.p1().y() );

        auto x2 = float( line.p2().x() );
        auto y2 = float( line.p2().y() );

        const bool steep = ( fabs( y2 - y1 ) > fabs( x2 - x1 ) );

        if( steep )
        {
            swap( x1, y1 );
            swap( x2, y2 );
        }

        if( x1 > x2 )
        {
            swap( x1, x2 );
            swap( y1, y2 );
        }

        const float dx = x2 - x1;
        const float dy = fabs( y2 - y1 );

        float error = dx / 2.0f;
        const int y_step = ( y1 < y2 ) ? 1 : -1;
        int y = int( y1 );

        const int max_x = int( x2 );

        for( int x = int( x1 ); x <= max_x; x++ )
        {
            if( steep )
                points.push_back( { y, x } );
            else
                points.push_back( { x, y } );

            error -= dy;
            if( error < 0 )
            {
                y += y_step;
                error += dx;
            }
        }

        return points;
    }
}
