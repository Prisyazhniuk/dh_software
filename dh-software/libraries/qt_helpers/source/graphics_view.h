#pragma once

#include "graphics_view_wheel_scroll.h"

#include <QFrame>
#include <QSlider>
#include <QToolButton>

class graphics_view : public QFrame
{
    Q_OBJECT

public:
    explicit graphics_view( QWidget *parent = nullptr );

    QGraphicsView* view() const;

public slots:
    void zoom_in( int zoom_step = 1 );
    void zoom_out( int zoom_step = 1 );

private slots:
    void reset_view();
    void set_reset_button_enabled();
    void setup_matrix();
    void toggle_open_gl();
    void toggle_antialiasing();

private:
    graphics_view_wheel_scroll* _graphics_view;
    QToolButton* _select_mode_button;
    QToolButton* _drag_mode_button;
    QToolButton* _open_gl_button;
    QToolButton* _antialias_button;
    QToolButton* _reset_button;
    QSlider* _zoom_slider;
};
