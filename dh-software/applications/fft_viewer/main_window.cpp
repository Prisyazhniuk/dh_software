#include "main_window.h"
#include "ui_main_window.h"

#include "exceptions/argument_exception.h"

#include <QFileDialog>
#include <QDir>

main_window::main_window( fft_viewer_core* core,
						  QWidget *parent )
	: QMainWindow( parent )
	, _core( core )
	, _ui( new Ui::main_window )
{
	if( !core )
		throw argument_exception( "core is null", get_exception_source() );

	_ui->setupUi( this );
}

main_window::~main_window()
{
	delete _ui;
}

void main_window::on_open_image_action_triggered()
{
	auto file_name = QFileDialog::getOpenFileName( this,
												   "Открыть изображение",
												   QDir::currentPath(),
												   "Изображения (*.bmp *.png)" );
}
