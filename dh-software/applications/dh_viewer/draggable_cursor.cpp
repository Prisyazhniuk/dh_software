#include "draggable_cursor.h"

#include <QtWidgets>

namespace dh
{
    draggable_cursor::draggable_cursor()
    {
        setFlags( ItemIsMovable | ItemSendsGeometryChanges | ItemIgnoresTransformations );
        setAcceptHoverEvents( true );
    }

    QRectF draggable_cursor::boundingRect() const
    {
        auto w = _pen_width;
        return QRectF( -10-w, -10-w, 21+2*w, 21+2*w );
    }

    void draggable_cursor::paint( QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* )
    {
        painter->setPen( QPen( Qt::red, _pen_width ) );
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
            {
                new_position.setX( qMin(rect.right(), qMax( new_position.x(), rect.left() ) ) );
                new_position.setY( qMin(rect.bottom(), qMax( new_position.y(), rect.top() ) ) );
                return new_position;
            }
        }

        return QGraphicsItem::itemChange( change, value );
    }

    void draggable_cursor::hoverEnterEvent( QGraphicsSceneHoverEvent* )
    {
        setCursor( Qt::CrossCursor );
    }
}
