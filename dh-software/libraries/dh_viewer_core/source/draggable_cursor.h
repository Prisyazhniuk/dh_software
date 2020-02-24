#pragma once

#include <QGraphicsItem>

namespace dh
{
    class draggable_cursor : public QObject, public QGraphicsItem
    {
        Q_OBJECT
        Q_INTERFACES( QGraphicsItem )

    public:
        draggable_cursor();

        void verify_position();

        QRectF boundingRect() const override;
        void paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* ) override;

    signals:
        void moved( const QPointF& );

    protected:
        QVariant itemChange( GraphicsItemChange, const QVariant& ) override;
        void hoverEnterEvent( QGraphicsSceneHoverEvent* ) override;
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* ) override;

    private:
        QPointF fit_to_rect( const QPointF&, const QRectF& rect );

    private:
        const QColor _color = Qt::red;
        const qreal _pen_width = 3;
    };
}
