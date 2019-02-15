#pragma once

#include "graphics_view_wheel_scroll.h"

#include <QFrame>
#include <QSlider>

namespace dh
{
    class graphics_view : public QFrame
    {
        Q_OBJECT

    public:
        explicit graphics_view( QWidget* parent = nullptr );

        void set_scene( QGraphicsScene* );

    public slots:
        void zoom_in( int zoom_step = 1 );
        void zoom_out( int zoom_step = 1 );
        void zoom_reset();

    private slots:
        void zoom();

    private:
        graphics_view_wheel_scroll* _graphics_view;
        QSlider* _zoom_slider;

        static constexpr int _max_zoom = 500;
        static constexpr int _initional_zoom = _max_zoom / 2;
        static constexpr qreal _zoom_devider = 50;
    };
}
