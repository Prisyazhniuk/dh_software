#pragma once

#include <QGraphicsView>

class graphics_view_wheel_scroll : public QGraphicsView
{
    Q_OBJECT

signals:
    void zoom_in( int zoom_step );
    void zoom_out( int zoom_step );

private:
    static const int _zoom_step = 6;

protected:
#if QT_CONFIG( wheelevent )
    void wheelEvent( QWheelEvent* ) override;
#endif
};
