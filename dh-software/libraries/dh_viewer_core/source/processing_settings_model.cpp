#include "processing_settings_model.h"

namespace dh
{
    processing_settings_model::processing_settings_model( const processing_settings& s,
                                                          QObject* parent )
        : QAbstractTableModel ( parent )
    {
        _settings.store( s );
    }

    int processing_settings_model::rowCount( const QModelIndex& ) const
    {
        return _rows;
    }

    int processing_settings_model::columnCount( const QModelIndex& ) const
    {
        return _cols;
    }

    QVariant processing_settings_model::data(const QModelIndex& index, int role ) const
    {
        if( !index.isValid() )
            return QVariant();

        auto row = index.row();
        auto col = index.column();

        if( row >= _rows || row < 0 )
            return QVariant();

        if( col >= _cols || col < 0 )
            return QVariant();

        if( role == Qt::DisplayRole || role == Qt::EditRole )
        {
            if( col == 0 )
            {
                switch( row )
                {
                    case 0: return "Длина волны (мм)";
                    case 1: return "Ширина сенсора (мм)";
                    case 2: return "Высота сенсора (мм)";
                    case 3: return "Дистанция (мм)";
                    case 4: return "Угол падения опорной волны (рад)";
                    default: QVariant();
                }
            }
            else
            {
                auto s = _settings.load();

                switch( row )
                {
                    case 0: return double( s.lambda_mm );
                    case 1: return double( s.sensor_width_mm );
                    case 2: return double( s.sensor_height_mm );
                    case 3: return double( s.distance_mm );
                    case 4: return double( s.theta_rad );
                    default: QVariant();
                }
            }

            return QVariant();
        }
        else
        {
            return QVariant();
        }
    }

    bool processing_settings_model::setData( const QModelIndex& index, const QVariant& value, int role )
    {
        if( !index.isValid() )
            return false;

        if( role != Qt::EditRole )
            return false;

        auto row = index.row();
        auto col = index.column();

        if( col < 1 )
            return false;

        auto s = _settings.load();

        switch( row )
        {
        case 0:
            s.lambda_mm = value.toFloat();
            break;
        case 1:
            s.sensor_width_mm = value.toFloat();
            break;
        case 2:
            s.sensor_height_mm = value.toFloat();
            break;
        case 3:
            s.distance_mm = value.toFloat();
            break;
        case 4:
            s.theta_rad = value.toFloat();
            break;
        default:
            return false;
        }

        _settings.store( s );

        emit dataChanged( index, index );

        return true;
    }

    Qt::ItemFlags processing_settings_model::flags( const QModelIndex& index ) const
    {
        if( !index.isValid() )
            return Qt::ItemIsEnabled;

        if( index.column() < 1 )
            return Qt::ItemIsEnabled;

        return QAbstractTableModel::flags( index ) | Qt::ItemIsEditable;
    }
}
