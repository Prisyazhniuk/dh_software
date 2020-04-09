#include "intensity_graph_widget.h"

#include <QLineSeries>
#include <QValueAxis>
#include <QMenu>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>

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

        setContextMenuPolicy( Qt::CustomContextMenu );

        connect( this, &intensity_graph_widget::customContextMenuRequested,
                 this, &intensity_graph_widget::show_context_menu ) ;
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

    void intensity_graph_widget::show_context_menu( const QPoint& position )
    {
        QMenu menu( "График интенсивности", this );

        QAction save_action( "Сохранить...", this );
        connect( &save_action, &QAction::triggered, this, &intensity_graph_widget::context_menu_save_action );
        save_action.setEnabled( !empty() );
        menu.addAction( &save_action );

        menu.exec( mapToGlobal(position) );
    }

    void intensity_graph_widget::context_menu_save_action()
    {
        static QString path =  QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation );

        QFileDialog dialog( this, "Сохранить данные" );
        dialog.setAcceptMode( QFileDialog::AcceptSave );
        dialog.setDirectory( path );
        dialog.setNameFilter( "CSV (*.csv);;Все файлы (*.*)" );
        dialog.setFileMode( QFileDialog::AnyFile );

        if( dialog.exec() != QDialog::Accepted )
            return;

        path = dialog.selectedFiles().first();

        auto data = export_to_csv();

        QFile file( path );
        if( file.open( QIODevice::WriteOnly ) )
        {
            file.write( data.toUtf8() );
            file.close();
        }
    }

    bool intensity_graph_widget::empty()
    {
        return _chart->series().empty();
    }

    QString intensity_graph_widget::export_to_csv()
    {
        QString csv = "x;y\r\n";

        for( auto& series : _chart->series() )
        {
            if( series->type() != QAbstractSeries::SeriesTypeLine )
                continue;

            auto s = qobject_cast<QLineSeries*>( series );
            if( !s )
                continue;

            for( auto p : s->points() )
                csv += QString("%1;%2\r\n").arg( p.x() ).arg( p.y() );
        }

        return csv.replace( '.', ',' );
    }
}
