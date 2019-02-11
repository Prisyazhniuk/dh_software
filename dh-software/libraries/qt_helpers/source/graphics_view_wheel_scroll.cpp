#include "graphics_view_wheel_scroll.h"

#if QT_CONFIG( wheelevent )
    #include <QWheelEvent>
#endif

graphics_view_wheel_scroll::graphics_view_wheel_scroll( QWidget* parent )
    : QGraphicsView ( parent )
{}

#if QT_CONFIG( wheelevent )
void graphics_view_wheel_scroll::wheelEvent( QWheelEvent* e )
{
    if( e->modifiers() & Qt::ControlModifier )
    {
        if( e->delta() > 0 )
            emit zoom_in( _zoom_step );
        else
            emit zoom_out( _zoom_step );

        e->accept();
    }
    else
    {
        QGraphicsView::wheelEvent( e );
    }
}
#endif
