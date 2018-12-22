#include "main_window.h"
#include "ui_main_window.h"

#include <QFileDialog>
#include <QDir>

main_window::main_window( QWidget *parent )
	: QMainWindow( parent )
	, ui( new Ui::main_window )
{
	ui->setupUi( this );
}

main_window::~main_window()
{
	delete ui;
}

void main_window::on_open_image_action_triggered()
{
	auto file_name = QFileDialog::getOpenFileName( this,
												   "Открыть изображение",
												   QDir::currentPath(),
												   "Изображения (*.bmp *.png)" );
}
