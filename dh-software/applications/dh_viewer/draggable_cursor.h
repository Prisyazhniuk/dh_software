#pragma once

#include <QGraphicsItem>

namespace dh
{
    class draggable_cursor : public QGraphicsItem
    {
    public:
        draggable_cursor();

        QRectF boundingRect() const override;
        void paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* ) override;

    protected:
        QVariant itemChange( GraphicsItemChange, const QVariant& ) override;
        void hoverEnterEvent( QGraphicsSceneHoverEvent* ) override;

    private:
        const QColor _color = Qt::red;
        const qreal _pen_width = 3;
    };
}
