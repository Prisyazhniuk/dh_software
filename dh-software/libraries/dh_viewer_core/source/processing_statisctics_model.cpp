#include "processing_statisctics_model.h"

namespace dh
{
    processing_statisctics_model::processing_statisctics_model( QObject* parent )
        : QAbstractTableModel ( parent )
        , _fft_processing_statistics( {} )
    {}

    int processing_statisctics_model::rowCount( const QModelIndex& ) const
    {
        return _rows;
    }

    int processing_statisctics_model::columnCount( const QModelIndex& ) const
    {
        return _cols;
    }

    QVariant processing_statisctics_model::data(const QModelIndex& index, int role ) const
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
            if( col == 0 )
                return QString( "Fps" );
            else
                return QString::number( fps(), 'f', 1 );
        }
        else
        {
            return QVariant();
        }
    }

    void processing_statisctics_model::update_statistics( const fft_processing_statistics& s )
    {
        _fft_processing_statistics = s;

        emit dataChanged( createIndex( 0, 0 ),
                          createIndex( _rows-1, _cols-1 ),
                          { Qt::DisplayRole } );
    }

    double processing_statisctics_model::fps() const
    {
        if( _fft_processing_statistics.period_us == 0 )
            return 0;

        return _fft_processing_statistics.frames_processed * 1000 * 1000.0 / _fft_processing_statistics.period_us;
    }
}
