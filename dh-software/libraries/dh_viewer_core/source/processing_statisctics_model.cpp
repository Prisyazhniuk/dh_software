#include "processing_statisctics_model.h"

namespace dh
{
    processing_statisctics_model::processing_statisctics_model( QObject* parent )
        : QAbstractTableModel ( parent )
        , _fft_processing_statistics( {} )
        , _blob_detection_statistics( {} )
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
            auto& s = _blob_detection_statistics;

            if( row == 0 )
                return col == 0 ? QString( "Время обработки (мс)" ) :
                                  QString::number( s.time_us/1000.0, 'f', 1 );

            if( row == 1 )
            {
                if( s.blobs_count )
                    return col == 0 ? QString( "Пучок 1" ) :
                                      QString::number( s.blobs[0].center_x ) + ", " + QString::number( s.blobs[0].center_y );
                else
                    return QVariant();
            }

            if( row == 2 )
            {
                if( s.blobs_count > 1 )
                    return col == 0 ? QString( "Пучок 2" ) :
                                      QString::number( s.blobs[1].center_x ) + ", " + QString::number( s.blobs[1].center_y );
                else
                    return QVariant();
            }

            return QVariant();

            //if( col == 0 )
            //    return QString( "Fps" );
            //else
            //    return QString::number( fps(), 'f', 1 );
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

    void processing_statisctics_model::update_statistics( const blob_detection_statistics& s )
    {
        _blob_detection_statistics = s;

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
