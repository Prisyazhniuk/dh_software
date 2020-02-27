#include "intensity_graph_widget.h"

#include <QLineSeries>

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
        _chart->removeAllSeries();

        auto series = new QLineSeries();

        int n = 0;
        auto rect = image.rect();
        for( auto p: points )
        {
            if( !rect.contains( p ) )
                continue;

            auto px = qGray( image.pixel( p ) );
            series->append( n++, px );
        }

        _chart->addSeries( series );
        _chart->createDefaultAxes();
    }

    void intensity_graph_widget::clean()
    {
        _chart->removeAllSeries();

        auto axes = _chart->axes();
        for( auto a : axes )
            _chart->removeAxis( a );
    }
}
