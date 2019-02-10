#include "graphics_view.h"

#ifndef QT_NO_OPENGL
    #include <QtOpenGL>
#else
    #include <QtWidgets>
#endif

#include <qmath.h>

#if QT_CONFIG( wheelevent )
void graphics_view_wheel_scroll::wheelEvent( QWheelEvent* e )
{
    if( e->modifiers() & Qt::ControlModifier )
    {
        if( e->delta() > 0 )
            _view->zoom_in( 6 );
        else
            _view->zoom_out( 6 );
        e->accept();
    }
    else
    {
        QGraphicsView::wheelEvent( e );
    }
}
#endif

graphics_view::graphics_view( const QString& name, QWidget* parent )
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

    int size = style()->pixelMetric( QStyle::PM_ToolBarIconSize );
    QSize icon_size( size, size );

    auto zoom_in_icon = new QToolButton;
    zoom_in_icon->setAutoRepeat( true );
    zoom_in_icon->setAutoRepeatInterval( 33 );
    zoom_in_icon->setAutoRepeatDelay( 0 );
    zoom_in_icon->setIcon( QPixmap(":/icons/zoom_in.png") );
    zoom_in_icon->setIconSize( icon_size );

    auto zoom_out_icon = new QToolButton;
    zoom_out_icon->setAutoRepeat( true );
    zoom_out_icon->setAutoRepeatInterval( 33 );
    zoom_out_icon->setAutoRepeatDelay( 0 );
    zoom_out_icon->setIcon( QPixmap(":/icons/zoom_out.png") );
    zoom_out_icon->setIconSize( icon_size );

    _zoom_slider = new QSlider;
    _zoom_slider->setMinimum( 0 );
    _zoom_slider->setMaximum( 500 );
    _zoom_slider->setValue( 250 );
    _zoom_slider->setTickPosition( QSlider::TicksRight );

    // Zoom slider layout
    auto zoom_slider_layout = new QVBoxLayout;
    zoom_slider_layout->addWidget( zoom_in_icon );
    zoom_slider_layout->addWidget( _zoom_slider );
    zoom_slider_layout->addWidget( zoom_out_icon );

    _reset_button = new QToolButton;
    _reset_button->setText( "0" );
    _reset_button->setEnabled( false );

    // Label layout
    auto label_layout = new QHBoxLayout;
    _label = new QLabel( name );
    _label2 = new QLabel( "Pointer Mode" );
    _select_mode_button = new QToolButton;
    _select_mode_button->setText( "Select" );
    _select_mode_button->setCheckable( true );
    _select_mode_button->setChecked( true );
    _drag_mode_button = new QToolButton;
    _drag_mode_button->setText( "Drag" );
    _drag_mode_button->setCheckable( true );
    _drag_mode_button->setChecked( false );
    _antialias_button = new QToolButton;
    _antialias_button->setText( "Antialiasing" );
    _antialias_button->setCheckable( true );
    _antialias_button->setChecked( false );
    _open_gl_button = new QToolButton;
    _open_gl_button->setText( "OpenGL" );
    _open_gl_button->setCheckable( true );
#ifndef QT_NO_OPENGL
    _open_gl_button->setEnabled( QGLFormat::hasOpenGL() );
#else
    openGlButton->setEnabled( false );
#endif

    label_layout->addWidget( _label );
    label_layout->addStretch();
    label_layout->addWidget( _label2 );
    label_layout->addWidget( _select_mode_button );
    label_layout->addWidget( _drag_mode_button );
    label_layout->addStretch();
    label_layout->addWidget( _antialias_button );
    label_layout->addWidget( _open_gl_button );

    auto top_layout = new QGridLayout;
    top_layout->addLayout( label_layout, 0, 0 );
    top_layout->addWidget( _graphics_view, 1, 0 );
    top_layout->addLayout( zoom_slider_layout, 1, 1 );
    top_layout->addWidget( _reset_button, 2, 1 );
    setLayout( top_layout );

    connect( _reset_button, SIGNAL(clicked()), this, SLOT(reset_view()) );
    connect( _zoom_slider, SIGNAL(valueChanged(int)), this, SLOT(setup_matrix()) );
    connect( _graphics_view->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(set_reset_button_enabled()) );
    connect( _graphics_view->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(set_reset_button_enabled()) );
    connect( _antialias_button, SIGNAL(toggled(bool)), this, SLOT(toggle_antialiasing()) );
    connect( _open_gl_button, SIGNAL(toggled(bool)), this, SLOT(toggle_open_gl()) );
    connect( zoom_in_icon, SIGNAL(clicked()), this, SLOT(zoom_in()) );
    connect( zoom_out_icon, SIGNAL(clicked()), this, SLOT(zoom_out()) );

    setup_matrix();
}

QGraphicsView* graphics_view::view() const
{
    return static_cast<QGraphicsView *>( _graphics_view );
}

void graphics_view::reset_view()
{
    _zoom_slider->setValue( 250 );
    setup_matrix();
    _graphics_view->ensureVisible( QRectF(0, 0, 0, 0) );

    _reset_button->setEnabled( false );
}

void graphics_view::set_reset_button_enabled()
{
    _reset_button->setEnabled( true );
}

void graphics_view::setup_matrix()
{
    qreal scale = qPow( qreal(2), ( _zoom_slider->value() - 250 ) / qreal(50) );

    QMatrix matrix;
    matrix.scale( scale, scale );

    _graphics_view->setMatrix( matrix );
    set_reset_button_enabled();
}

void graphics_view::toggle_open_gl()
{
#ifndef QT_NO_OPENGL
    _graphics_view->setViewport( _open_gl_button->isChecked() ? new QGLWidget( QGLFormat(QGL::SampleBuffers) ) : new QWidget );
#endif
}

void graphics_view::toggle_antialiasing()
{
    _graphics_view->setRenderHint( QPainter::Antialiasing, _antialias_button->isChecked() );
}

void graphics_view::zoom_in( int level )
{
    _zoom_slider->setValue( _zoom_slider->value() + level );
}

void graphics_view::zoom_out( int level )
{
    _zoom_slider->setValue( _zoom_slider->value() - level );
}
