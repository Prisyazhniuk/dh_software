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

    _ui->vertical_splitter->setStretchFactor( 0, 1 );
    _ui->vertical_splitter->setStretchFactor( 1, 3 );
    _ui->vertical_splitter->setCollapsible( 1, false );

    _ui->horizontal_splitter->setStretchFactor( 0, 3 );
    _ui->horizontal_splitter->setStretchFactor( 1, 1 );
    _ui->horizontal_splitter->setCollapsible( 0, false );
    _ui->horizontal_splitter->setCollapsible( 1, false );

    _scene = new QGraphicsScene( this );
    _scene_item = _scene->addPixmap( QPixmap() );

    _graphics_view = new graphics_view( this );
    _graphics_view->set_scene( _scene );
    _ui->graphics_view_layout->addWidget( _graphics_view );

    auto images_files_filter = make_images_files_filter();
    _file_system_model = new QFileSystemModel( this );
    _file_system_model->setRootPath( "" );
    _file_system_model->setNameFilters( images_files_filter );
    _file_system_model->setNameFilterDisables( false );
    _ui->files_tree_view->setModel( _file_system_model );
    // _ui->files_tree_view->setRootIndex( last_opened );
    for( int i = 1; i < _file_system_model->columnCount(); i++ )
        _ui->files_tree_view->hideColumn( i );

    _processing_statistics_model = new processing_statisctics_model( this );
    _ui->statistics_view->setModel( _processing_statistics_model );
    _ui->statistics_view->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    _ui->statistics_view->horizontalHeader()->hide();
    _ui->statistics_view->verticalHeader()->hide();

    connect( this, qOverload<const fft_processing_statistics&>( &main_window::update_statistics_model ),
             _processing_statistics_model, qOverload<const fft_processing_statistics&>( &processing_statisctics_model::update_statistics ) );

    connect( this, qOverload<const blob_detection_statistics&>( &main_window::update_statistics_model ),
             _processing_statistics_model, qOverload<const blob_detection_statistics&>( &processing_statisctics_model::update_statistics ) );

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

void main_window::statistics_ready( const blob_detection_statistics& s )
{
    emit update_statistics_model( s );
}

void main_window::on_open_image_action_triggered()
{
    QFileDialog dialog( this, "Открыть изображение" );

    dialog.setDirectory( QDir::currentPath() );

    auto all_images_types = make_images_files_filter();
    auto all_images_filter = QString( "Все файлы изображений (%1)" ).arg( all_images_types.join(' ') );
    auto all_files_filter = QString( "Все файлы (*)" );

    dialog.setMimeTypeFilters( _supported_file_types );

    auto filters = dialog.nameFilters();
    filters.append( all_images_filter );
    filters.append( all_files_filter );
    dialog.setNameFilters(filters);

    dialog.selectNameFilter(all_images_filter);

    if( dialog.exec() == QDialog::Accepted )
    {
        auto file_path = dialog.selectedFiles().first();
        //_fft_processor->run( file_path.toStdString() );
        _blob_detector->run( file_path.toStdString() );
    }
}

QStringList main_window::make_images_files_filter()
{
    QMimeDatabase mime_database;
    QStringList all_images_types;
    for( auto& filter: _supported_file_types )
    {
        auto mime_type = mime_database.mimeTypeForName( filter );
        if( mime_type.isValid() )
            all_images_types.append( mime_type.globPatterns() );
    }

    return all_images_types;
}

void main_window::on_files_tree_view_activated( const QModelIndex& index )
{
    if( _file_system_model->isDir( index ) )
        return;

    auto file_info = _file_system_model->fileInfo( index );
    auto file_path = file_info.absoluteFilePath();

    //_fft_processor->run( file_path.toStdString() );
    _blob_detector->run( file_path.toStdString() );
}
