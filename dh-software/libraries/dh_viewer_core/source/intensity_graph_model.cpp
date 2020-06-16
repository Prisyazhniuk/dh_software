#include "intensity_graph_model.h"
#include "bresenham_line_algorithm.h"

#include <QRegularExpression>
#include <QPainter>

namespace dh
{
    intensity_graph_model::intensity_graph_model( QGraphicsScene* scene,
                                                  QGraphicsPixmapItem* scene_item,
                                                  QObject* parent )
        : QAbstractTableModel( parent )
        , _first_enable( true )
        , _scene( scene )
        , _scene_item( scene_item )
        , _enabled( false )
        , _cursor_1( new draggable_cursor() )
        , _cursor_2( new draggable_cursor() )
        , _line ( new QGraphicsLineItem() )
    {
        _cursor_1->setPos( 0, 0 );
        _cursor_2->setPos( 0, 0 );

        _line->setPen( QPen( _color, _pen_width ) );

        connect( _cursor_1, &draggable_cursor::moved, this, &intensity_graph_model::cursor_moved );
        connect( _cursor_2, &draggable_cursor::moved, this, &intensity_graph_model::cursor_moved );
    }

    int intensity_graph_model::rowCount( const QModelIndex& ) const
    {
        return _rows;
    }

    int intensity_graph_model::columnCount( const QModelIndex& ) const
    {
        return _cols;
    }

    QVariant intensity_graph_model::data(const QModelIndex& index, int role ) const
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
                    case 0: return "Включить";
                    case 1: return "Точка 1";
                    case 2: return "Точка 2";
                }
            }
            else
            {
                switch( row )
                {
                    case 1: return toString( _cursor_1->pos() );
                    case 2: return toString( _cursor_2->pos() );
                }
            }

            return QVariant();
        }
        else if( role == Qt::CheckStateRole )
        {
            if( col == 1 )
            {
                switch( row )
                {
                    case 0: return _enabled ? Qt::Checked : Qt::Unchecked;
                }
            }

            return QVariant();
        }
        else
        {
            return QVariant();
        }
    }

    bool intensity_graph_model::setData( const QModelIndex& index, const QVariant& value, int role )
    {
        if( !index.isValid() )
            return false;

        if( role != Qt::EditRole && role != Qt::CheckStateRole )
            return false;

        auto row = index.row();
        auto col = index.column();

        if( col < 1 )
            return false;

        switch( row )
        {
            case 0:
                _enabled = value.toBool();

                if( _enabled )
                    enable();
                else
                    disable();

                break;
            case 1:
            {
                try
                {
                    auto point = toPoint( value.toString() );
                    _cursor_1->setPos( point );
                    calculate_intensity();
                    break;
                }
                catch( ... )
                {
                    return false;
                }
            }
            case 2:
            {
                try
                {
                    auto point = toPoint( value.toString() );
                    _cursor_2->setPos( point );
                    calculate_intensity();
                    break;
                }
                catch( ... )
                {
                    return false;
                }
            }
            default:
                return false;
        }

        emit dataChanged( index, index );

        return true;
    }

    Qt::ItemFlags intensity_graph_model::flags( const QModelIndex& index ) const
    {
        if( !index.isValid() )
            return Qt::ItemIsEnabled;

        if( index.column() < 1 )
            return Qt::ItemIsEnabled;

        if( index.row() == 0 )
            return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;

        if( index.row() == 1 || index.row() == 2 )
        {
            auto is_editable = _enabled ? Qt::ItemIsEditable : Qt::NoItemFlags;
            return QAbstractTableModel::flags( index ) | is_editable;
        }

        return QAbstractTableModel::flags( index );
    }

    void intensity_graph_model::image_processed( const QImage& image )
    {
        if( _first_enable )
        {
            _cursor_1->setPos( 0, image.height()/2 );
            _cursor_2->setPos( image.width()-1, image.height()/2 );

            emit dataChanged( createIndex( 0, 0 ), createIndex( _rows-1, _cols-1 ) );
            if( _enabled )
                calculate_intensity();
        }
        else
        {
            _cursor_1->verify_position();
            _cursor_2->verify_position();
        }
    }

    void intensity_graph_model::cursor_moved( const QPointF& )
    {
        emit dataChanged( createIndex( 0, 0 ), createIndex( _rows-1, _cols-1 ) );
        calculate_intensity();
    }

    void intensity_graph_model::enable()
    {
        _first_enable = false;

        _scene->addItem( _cursor_1 );
        _scene->addItem( _cursor_2 );

        calculate_intensity();
        _scene->addItem( _line );
    }

    void intensity_graph_model::disable()
    {
        _scene->removeItem( _cursor_1 );
        _scene->removeItem( _cursor_2 );
        _scene->removeItem( _line );

        emit disabled();
    }

    void intensity_graph_model::calculate_intensity()
    {
        auto line = QLineF( _cursor_1->pos(), _cursor_2->pos() );
        _line->setLine( line );

        auto image = _scene_item->pixmap().toImage();
        auto points = bresenham_line_algorithm::calculate( line );

        emit plot( image, points );
    }

    QPointF intensity_graph_model::toPoint( const QString& s ) const
    {
        QRegularExpression re( "^\\s*\\(?\\s*(\\d+)\\s*[,;]\\s*(\\d+)\\s*\\)?\\s*$" );
        auto match = re.match( s );

        if( match.hasMatch() )
        {
            auto x = match.captured( 1 ).toInt();
            auto y = match.captured( 2 ).toInt();
            return QPointF(x, y);
        }
        else
            throw argument_exception( get_exception_source() );
    }

    QString intensity_graph_model::toString( const QPointF& p ) const
    {
        return QString("(%1, %2)").arg( int(p.x()) ).arg( int(p.y()) );
    }
}
