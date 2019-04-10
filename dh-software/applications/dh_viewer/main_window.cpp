#include "main_window.h"
#include "ui_main_window.h"

#include "exceptions/argument_exception.h"

#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QDir>
#include <QMimeDatabase>

using namespace dh;

main_window::main_window( fft_processor* fft_processor,
                          blob_detector* blob_detector,
                          QWidget *parent )
    : QMainWindow( parent )
    , _fft_processor( fft_processor )
    , _blob_detector( blob_detector )
    , _ui( new Ui::main_window )
{
    if( !fft_processor )
        throw argument_exception( "fft_processor is null", get_exception_source() );

    if( !blob_detector )
        throw argument_exception( "blob_detector is null", get_exception_source() );

    _ui->setupUi( this );

    _ui->splitter->setStretchFactor( 0, 1 );
    _ui->splitter->setStretchFactor( 1, 3 );
    _ui->splitter->setCollapsible( 1, false );

    _scene = new QGraphicsScene( this );
    _scene_item = _scene->addPixmap( QPixmap() );

    _graphics_view = new graphics_view( this );
    _graphics_view->set_scene( _scene );
    _ui->graphics_view_layout->addWidget( _graphics_view );

    _processing_statistics_model = new processing_statisctics_model( this );
    _ui->statistics_view->setModel( _processing_statistics_model );
    _ui->statistics_view->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    _ui->statistics_view->horizontalHeader()->hide();
    _ui->statistics_view->verticalHeader()->hide();

    connect( this, &main_window::update_statistics_model,
             _processing_statistics_model, &processing_statisctics_model::update_statistics );

    showMaximized();
}

main_window::~main_window()
{
    _fft_processor->stop();
    _blob_detector->stop();
    delete _ui;
}

void main_window::image_processed( const QImage& image )
{
    auto pixmap = QPixmap::fromImage( image );

    if( _scene_item->pixmap().rect() != pixmap.rect() )
        _scene->setSceneRect( pixmap.rect() );

    _scene_item->setPixmap( pixmap );
}

void main_window::statistics_ready( const fft_processing_statistics& s )
{
    emit update_statistics_model( s );
}

void main_window::on_open_image_action_triggered()
{
    QFileDialog dialog( this, "Открыть изображение" );

    dialog.setDirectory( QDir::currentPath() );

    QStringList type_filters;
    type_filters.push_back( "image/png" );
    type_filters.push_back( "image/jpeg" );
    type_filters.push_back( "image/bmp" );

    QMimeDatabase mime_database;
    QStringList all_images_types;
    for( auto& filter: type_filters )
    {
        auto mime_type = mime_database.mimeTypeForName( filter );
        if( mime_type.isValid() )
            all_images_types.append( mime_type.globPatterns() );
    }

    auto all_images_filter = QString( "Все файлы изображений (%1)" ).arg( all_images_types.join(' ') );
    auto all_files_filter = QString( "Все файлы (*)" );

    dialog.setMimeTypeFilters( type_filters );

    auto filters = dialog.nameFilters();
    filters.append( all_images_filter );
    filters.append( all_files_filter );
    dialog.setNameFilters(filters);

    dialog.selectNameFilter(all_images_filter);

    if( dialog.exec() == QDialog::Accepted )
    {
        auto file_name = dialog.selectedFiles().first();
        //_fft_processor->run( file_name.toStdString() );
        _blob_detector->run( file_name.toStdString() );
    }
}
