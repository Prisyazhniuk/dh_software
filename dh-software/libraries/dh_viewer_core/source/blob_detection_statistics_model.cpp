#include "blob_detection_statistics_model.h"

namespace dh
{
    blob_detection_statistics_model::blob_detection_statistics_model( QObject* parent )
        : QAbstractTableModel ( parent )
    {
        _statistics.store( {} );
    }

    int blob_detection_statistics_model::rowCount( const QModelIndex& ) const
    {
        return _rows;
    }

    int blob_detection_statistics_model::columnCount( const QModelIndex& ) const
    {
        return _cols;
    }

    QVariant blob_detection_statistics_model::data(const QModelIndex& index, int role ) const
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
        }
        else
        {
            return QVariant();
        }
    }

    void blob_detection_statistics_model::update_statistics( const blob_detection_statistics& s )
    {
        _statistics = s;

        emit dataChanged( createIndex( 0, 0 ), createIndex( _rows-1, _cols-1 ), { Qt::DisplayRole } );
    }    
}
