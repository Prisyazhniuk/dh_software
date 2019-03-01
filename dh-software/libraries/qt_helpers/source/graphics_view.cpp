#include "graphics_view.h"

#ifndef QT_NO_OPENGL
    #include <QtOpenGL>
#else
    #include <QtWidgets>
#endif

#include <qmath.h>

namespace dh
{
    graphics_view::graphics_view( QWidget* parent )
        : QFrame( parent )
    {
        setFrameStyle( Sunken | StyledPanel );

        _graphics_view = new graphics_view_wheel_scroll( this );
        _graphics_view->setRenderHint( QPainter::Antialiasing, false );
        _graphics_view->setOptimizationFlags( QGraphicsView::DontSavePainterState );
        _graphics_view->setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
        _graphics_view->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
        _graphics_view->setInteractive( true );
        _graphics_view->setDragMode( QGraphicsView::ScrollHandDrag );

        connect( _graphics_view, &graphics_view_wheel_scroll::zoom_in, this, &graphics_view::zoom_in );
        connect( _graphics_view, &graphics_view_wheel_scroll::zoom_out, this, &graphics_view::zoom_out );

        int size = style()->pixelMetric( QStyle::PM_ToolBarIconSize );
        QSize icon_size( size, size );

        const int repeat_interval = 30;

        auto zoom_in_button = new QToolButton();
        zoom_in_button->setAutoRepeat( true );
        zoom_in_button->setAutoRepeatInterval( repeat_interval );
        zoom_in_button->setAutoRepeatDelay( 0 );
        zoom_in_button->setIcon( QPixmap(":/icons/zoom_in.png") );
        zoom_in_button->setIconSize( icon_size );

        auto zoom_reset_button = new QToolButton();
        zoom_reset_button->setIcon( QPixmap(":/icons/zoom_reset.png") );
        zoom_reset_button->setIconSize( icon_size );

        auto zoom_out_button = new QToolButton();
        zoom_out_button->setAutoRepeat( true );
        zoom_out_button->setAutoRepeatInterval( repeat_interval );
        zoom_out_button->setAutoRepeatDelay( 0 );
        zoom_out_button->setIcon( QPixmap(":/icons/zoom_out.png") );
        zoom_out_button->setIconSize( icon_size );

        _zoom_slider = new QSlider( Qt::Horizontal );
        _zoom_slider->setMinimum( 0 );
        _zoom_slider->setMaximum( _max_zoom );
        _zoom_slider->setValue( _initional_zoom );
        _zoom_slider->setTickPosition( QSlider::TicksBelow );

        auto zoom_layout = new QHBoxLayout();
        zoom_layout->addWidget( zoom_in_button );
        zoom_layout->addWidget( zoom_reset_button );
        zoom_layout->addWidget( zoom_out_button );
        zoom_layout->addWidget( _zoom_slider );
        zoom_layout->addStretch();

        auto top_layout = new QVBoxLayout();
        top_layout->addLayout( zoom_layout );
        top_layout->addWidget( _graphics_view );
        setLayout( top_layout );

        connect( _zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(zoom()) );

        connect( zoom_in_button, SIGNAL(clicked()), this, SLOT(zoom_in()) );
        connect( zoom_out_button, SIGNAL(clicked()), this, SLOT(zoom_out()) );
        connect( zoom_reset_button, SIGNAL(clicked()), this, SLOT(zoom_reset()) );

        zoom();
    }

    void graphics_view::set_scene( QGraphicsScene* scene )
    {
        _graphics_view->setScene( scene );
    }

    void graphics_view::zoom_in( int zoom_step )
    {
        _zoom_slider->setValue( _zoom_slider->value() + zoom_step );
    }

    void graphics_view::zoom_out( int zoom_step )
    {
        _zoom_slider->setValue( _zoom_slider->value() - zoom_step );
    }

    void graphics_view::zoom_reset()
    {
        _zoom_slider->setValue( _initional_zoom );
        zoom();

        auto center = _graphics_view->scene()->sceneRect().center();
        _graphics_view->centerOn( center );
    }

    void graphics_view::zoom()
    {
        qreal scale = qPow( qreal(2), ( _zoom_slider->value() - _initional_zoom ) / _zoom_devider );

        QMatrix matrix;
        matrix.scale( scale, scale );

        _graphics_view->setMatrix( matrix );
    }
}
