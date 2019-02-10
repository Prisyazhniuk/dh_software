#pragma once

#include <QFrame>
#include <QGraphicsView>
#include <QLabel>
#include <QSlider>
#include <QToolButton>

class graphics_view;

class graphics_view_wheel_scroll : public QGraphicsView
{
    Q_OBJECT

public:
    graphics_view_wheel_scroll( graphics_view* view )
        : QGraphicsView()
        , _view( view )
    {}

protected:
#if QT_CONFIG( wheelevent )
    void wheelEvent( QWheelEvent* ) override;
#endif

private:
    graphics_view* _view;
};

class graphics_view : public QFrame
{
    Q_OBJECT

public:
    explicit graphics_view( const QString& name, QWidget *parent = nullptr );

    QGraphicsView* view() const;

public slots:
    void zoom_in( int level = 1 );
    void zoom_out( int level = 1 );

private slots:
    void reset_view();
    void set_reset_button_enabled();
    void setup_matrix();
    void toggle_open_gl();
    void toggle_antialiasing();

private:
    graphics_view_wheel_scroll* _graphics_view;
    QLabel* _label;
    QLabel* _label2;
    QToolButton* _select_mode_button;
    QToolButton* _drag_mode_button;
    QToolButton* _open_gl_button;
    QToolButton* _antialias_button;
    QToolButton* _reset_button;
    QSlider* _zoom_slider;
};
