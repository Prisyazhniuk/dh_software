#include "main_window.h"
#include "ui_main_window.h"

#include "exceptions/argument_exception.h"

#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QDir>

using namespace dh;

main_window::main_window( fft_viewer_core* core,
						  QWidget *parent )
	: QMainWindow( parent )
	, _core( core )
	, _ui( new Ui::main_window )
{
	if( !core )
		throw argument_exception( "core is null", get_exception_source() );

	_ui->setupUi( this );

    _ui->splitter->setStretchFactor( 0, 1 );
    _ui->splitter->setStretchFactor( 1, 3 );
    _ui->splitter->setCollapsible( 1, false );

    _scene = new QGraphicsScene( this );
    _scene_item = _scene->addPixmap( QPixmap() );

    _graphics_view = new graphics_view( this );
    _graphics_view->set_scene( _scene );
    _ui->graphics_view_layout->addWidget( _graphics_view );

    showMaximized();
}

main_window::~main_window()
{
    _core->stop();
    delete _ui;
}

void main_window::show_image( const QImage& image )
{
    auto pixmap = QPixmap::fromImage( image );

    if( _scene_item->pixmap().rect() != pixmap.rect() )
        _scene->setSceneRect( pixmap.rect() );

    _scene_item->setPixmap( pixmap );
}

void main_window::on_open_image_action_triggered()
{
	auto file_name = QFileDialog::getOpenFileName( this,
												   "Открыть изображение",
												   QDir::currentPath(),
												   "Изображения (*.bmp *.png)" );

    _core->run( file_name.toStdString() );
}
