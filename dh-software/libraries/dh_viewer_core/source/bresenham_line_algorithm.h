#pragma once

#include <QList>
#include <QLineF>

namespace dh
{
    class bresenham_line_algorithm
    {
    public:
        static QList<QPoint> calculate( const QLineF& line );
    };
}
