#include "draggable_cursor.h"

#include <math.h>

#include <QtWidgets>

namespace dh
{
    draggable_cursor::draggable_cursor()
    {
        setFlags( ItemIsMovable | ItemSendsGeometryChanges | ItemIgnoresTransformations );
        setAcceptHoverEvents( true );
    }

    void draggable_cursor::verify_position()
    {
        if( scene() )
        {
            auto p = pos();
            auto rect = scene()->sceneRect();

            if( !rect.contains( p ) )
                p = fit_to_rect( p, rect );

            p = round_point( p );

            if( p != pos() )
            {
                setPos( p );
                emit moved( p );
            }
        }
    }

    QRectF draggable_cursor::boundingRect() const
    {
        auto w = _pen_width;
        return QRectF( -10-w, -10-w, 21+2*w, 21+2*w );
    }

    void draggable_cursor::paint( QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* )
    {
        painter->setPen( QPen( _color, _pen_width ) );
        painter->setBrush( QBrush( _color ) );
        painter->drawLine( -10, 0, 10, 0 );
        painter->drawLine( 0, -10, 0, 10 );
    }

    QVariant draggable_cursor::itemChange( GraphicsItemChange change, const QVariant& value )
    {
        if( change == ItemPositionChange && scene() )
        {
            auto new_position = value.toPointF();
            auto rect = scene()->sceneRect();

            if( !rect.contains( new_position ) )
                new_position = fit_to_rect( new_position, rect );

            new_position = round_point( new_position );

            return new_position;
        }

        return QGraphicsItem::itemChange( change, value );
    }

    void draggable_cursor::hoverEnterEvent( QGraphicsSceneHoverEvent* e )
    {
        setCursor( Qt::CrossCursor );
        QGraphicsItem::hoverEnterEvent( e );
    }

    void draggable_cursor::mouseReleaseEvent( QGraphicsSceneMouseEvent* e )
    {
        emit moved( pos() );
        QGraphicsItem::mouseReleaseEvent( e );
    }

    QPointF draggable_cursor::fit_to_rect( const QPointF& p, const QRectF& rect )
    {
        auto x = qMin( rect.right()-1, qMax( p.x(), rect.left() ) );
        auto y = qMin( rect.bottom()-1, qMax( p.y(), rect.top() ) );
        return QPointF( x, y );
    }

    QPointF draggable_cursor::round_point( QPointF p )
    {
        double i;

        if( modf( p.x(), &i ) != 0.5 )
            p.setX( 0.5 + round( p.x() ) );

        if( modf( p.y(), &i ) != 0.5 )
            p.setY( 0.5 + round( p.y() ) );

        return p;
    }
}
