#include "intensity_graph_widget.h"

#include <QLineSeries>
#include <QValueAxis>

#include <math.h>

namespace dh
{
    intensity_graph_widget::intensity_graph_widget( QWidget* parent )
        : QChartView( parent )
        , _chart( new QChart() )
    {
        _chart->legend()->hide();

        setChart( _chart );
        setRenderHint( QPainter::Antialiasing );
    }

    void intensity_graph_widget::plot( const QImage& image, const QList<QPoint>& points )
    {
        auto rect = image.rect();

        int min = 255;
        int max = 0;

        for( auto p: points )
        {
            if( !rect.contains( p ) )
                continue;

            auto px = qGray( image.pixel( p ) );

            if( px > max )
                max = px;

            if( px < min )
                min = px;
        }


        _chart->removeAllSeries();
        auto series = new QLineSeries();

        QPoint start_point = *points.begin();
        double image_width = image.width();
        double range = ( max - min ) != 0 ? max - min : 1;

        for( auto p: points )
        {
            if( !rect.contains( p ) )
                continue;

            qreal px = qGray( image.pixel( p ) );
            px = ( px - min ) / range;

            auto dx = start_point.x() - p.x();
            auto dy = start_point.y() - p.y();
            auto distance = sqrt( dx*dx + dy*dy );
            auto len = distance / image_width;

            series->append( len, px );
        }

        _chart->addSeries( series );
        _chart->createDefaultAxes();

        auto axes = _chart->axes( Qt::Horizontal );
        if( !axes.empty())
        {
            auto a = static_cast<QValueAxis *>( *axes.begin() );
            a->setTickCount( 5 );
        }
    }

    void intensity_graph_widget::clean()
    {
        _chart->removeAllSeries();

        auto axes = _chart->axes();
        for( auto a : axes )
            _chart->removeAxis( a );
    }
}
