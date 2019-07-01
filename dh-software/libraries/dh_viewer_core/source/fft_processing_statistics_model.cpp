#include "fft_processing_statistics_model.h"

namespace dh
{
    fft_processing_statistics_model::fft_processing_statistics_model( QObject* parent )
        : QAbstractTableModel ( parent )
    {
        _statistics.store( {} );
    }

    int fft_processing_statistics_model::rowCount( const QModelIndex& ) const
    {
        return _rows;
    }

    int fft_processing_statistics_model::columnCount( const QModelIndex& ) const
    {
        return _cols;
    }

    QVariant fft_processing_statistics_model::data(const QModelIndex& index, int role ) const
    {
        if( !index.isValid() )
            return QVariant();

        auto row = index.row();
        auto col = index.column();

        if( row >= _rows || row < 0 )
            return QVariant();

        if( col >= _cols || col < 0 )
            return QVariant();

        if( role == Qt::DisplayRole )
        {
            auto s = _statistics.load();

            if( row == 0 )
                return col == 0 ? QString( "Fps" ) :
                                  QString::number( fps(s), 'f', 1 );

            return QVariant();
        }
        else
        {
            return QVariant();
        }
    }

    void fft_processing_statistics_model::update_statistics( const fft_processing_statistics& s )
    {
        _statistics = s;

        emit dataChanged( createIndex( 0, 0 ),
                          createIndex( _rows-1, _cols-1 ),
                          { Qt::DisplayRole } );
    }

    double fft_processing_statistics_model::fps( const fft_processing_statistics& s )
    {
        if( s.period_us == 0 )
            return 0;

        return s.frames_processed * 1000 * 1000.0 / s.period_us;
    }
}
