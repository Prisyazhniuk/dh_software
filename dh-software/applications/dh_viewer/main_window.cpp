#include "main_window.h"
#include "ui_main_window.h"
#include "float_item_delegate.h"
#include "image_loader.h"

#include "exceptions/argument_exception.h"

#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QMimeDatabase>
#include <QTimer>
#include <QDir>
#include <QMessageBox>

using namespace dh;
using namespace std;

main_window::main_window( hologram_processor* hologram_processor,
                          QWidget *parent )
    : QMainWindow( parent )
    , _hologram_processor( hologram_processor )
    , _ui( new Ui::main_window )
    , _settings_working_path_key( "working_path" )
    , _settings_save_path_key( "save_path" )
{
    if( !hologram_processor )
        throw argument_exception( "hologram_processor is null", get_exception_source() );

    _settings = new QSettings( "dh", "dh_viewer", this );

    _ui->setupUi( this );

    _ui->vertical_splitter->setStretchFactor( 0, 1 );
    _ui->vertical_splitter->setStretchFactor( 1, 3 );
    _ui->vertical_splitter->setCollapsible( 1, false );

    _ui->horizontal_splitter->setStretchFactor( 0, 2 );
    _ui->horizontal_splitter->setStretchFactor( 1, 1 );
    _ui->horizontal_splitter->setStretchFactor( 1, 1 );
    _ui->horizontal_splitter->setCollapsible( 0, false );
    _ui->horizontal_splitter->setCollapsible( 1, false );
    _ui->horizontal_splitter->setCollapsible( 2, false );

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
    for( int i = 1; i < _file_system_model->columnCount(); i++ )
        _ui->files_tree_view->hideColumn( i );
    _ui->files_tree_view->setHeaderHidden( true );

    auto working_path = _settings->value( _settings_working_path_key, "" ).toString();
    scroll_files_tree_view( working_path );

    auto settings = processing_settings
    {
        .lambda_mm = 0.0006328f,
        .sensor_width_mm = 4.24f,
        .sensor_height_mm = 2.39f,
        .distance_mm = 23.2f,
        .theta_rad = 0
    };

    _processing_settings_model = new processing_settings_model( settings, this );
    _ui->settings_view->setModel( _processing_settings_model );
    _ui->settings_view->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    _ui->settings_view->horizontalHeader()->hide();
    _ui->settings_view->verticalHeader()->hide();
    _ui->settings_view->setItemDelegateForRow( 0, new float_item_delegate( 8, 0.0000001, _ui->settings_view ) ); // lambda_mm
    _ui->settings_view->setItemDelegateForRow( 1, new float_item_delegate( 3, 0.01, _ui->settings_view ) ); // sensor_width_mm
    _ui->settings_view->setItemDelegateForRow( 2, new float_item_delegate( 3, 0.01, _ui->settings_view ) ); // sensor_height_mm
    _ui->settings_view->setItemDelegateForRow( 3, new float_item_delegate( 2, 0.1, _ui->settings_view ) ); // distance_mm
    _ui->settings_view->setItemDelegateForRow( 4, new float_item_delegate( 3, 0.01, _ui->settings_view ) ); // theta_rad

    connect( _processing_settings_model, &processing_settings_model::dataChanged,
             this, &main_window::settings_changed );

    _processing_statistics_model = new processing_statistics_model( this );
    _ui->statistics_view->setModel( _processing_statistics_model );
    _ui->statistics_view->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    _ui->statistics_view->horizontalHeader()->hide();
    _ui->statistics_view->verticalHeader()->hide();

    connect( this, &main_window::update_statistics_model, _processing_statistics_model, &processing_statistics_model::update_statistics );

    showMaximized();
}

main_window::~main_window()
{
    _hologram_processor->stop();
    delete _ui;
}

void main_window::image_processed( const QImage& image )
{
    auto pixmap = QPixmap::fromImage( image );

    lock_guard<mutex> lock( _scene_item_mutex );

    if( _scene_item->pixmap().rect() != pixmap.rect() )
        _scene->setSceneRect( pixmap.rect() );

    _scene_item->setPixmap( pixmap );
}

void main_window::error_notified( const QString& message )
{
    QMessageBox::critical( this, "Ошибка", message );
}

void main_window::statistics_ready( const processing_statistics& s )
{
    emit update_statistics_model( s );
}

void main_window::on_open_image_action_triggered()
{
    auto working_path = _settings->value( _settings_working_path_key, "" ).toString();

    auto optional_file_path = execute_file_dialog( "Открыть изображение",
                                                   working_path,
                                                   QFileDialog::AcceptOpen,
                                                   QFileDialog::ExistingFile,
                                                   true );
    if( optional_file_path.has_value() )
    {
        try
        {
            auto file_path = optional_file_path.value();

            QFileInfo file_info( file_path );
            _settings->setValue( _settings_working_path_key, file_info.absolutePath() );
            scroll_files_tree_view( file_path );

            _source_image = image_loader::load( file_path );
            _hologram_processor->reconstruct( _source_image, _processing_settings_model->get() );
        }
        catch( QString& message )
        {
            error_notified( message );
        }
    }
}

void main_window::on_save_image_action_triggered()
{
    lock_guard<mutex> lock( _scene_item_mutex );

    if( _scene_item->pixmap().isNull() )
        return;

    auto save_path = _settings->value( _settings_save_path_key, "" ).toString();

    auto optional_file_path = execute_file_dialog( "Сохранить изображение",
                                                   save_path,
                                                   QFileDialog::AcceptSave,
                                                   QFileDialog::AnyFile,
                                                   false );
    if( optional_file_path.has_value() )
    {
        try
        {
            auto file_path = optional_file_path.value();

            QFileInfo file_info( file_path );
            _settings->setValue( _settings_save_path_key, file_info.absolutePath() );

            _scene_item->pixmap().save( file_path );
        }
        catch( QString& message )
        {
            error_notified( message );
        }
    }
}

void main_window::on_files_tree_view_activated( const QModelIndex& index )
{
    if( _file_system_model->isDir( index ) )
        return;

    try
    {
        auto file_info = _file_system_model->fileInfo( index );
        auto file_path = file_info.absoluteFilePath();

        _settings->setValue( _settings_working_path_key, file_info.absolutePath() );

        _source_image = image_loader::load( file_path );
        _hologram_processor->reconstruct( _source_image, _processing_settings_model->get() );
    }
    catch( QString& message )
    {
        error_notified( message );
    }
}

void main_window::settings_changed( const QModelIndex&, const QModelIndex&, const QVector<int>& )
{
    try
    {
        if( _source_image.empty() )
            throw QString( "Изображение не загружено" );

        _hologram_processor->reconstruct( _source_image, _processing_settings_model->get() );

    }
    catch( QString& message )
    {
        error_notified( message );
    }
}

optional<QString> main_window::execute_file_dialog( const QString& caption, const QString& path,
                                                    QFileDialog::AcceptMode accept_mode,
                                                    QFileDialog::FileMode file_mode, bool set_all_files_filter )
{
    QFileDialog dialog( this, caption );
    dialog.setAcceptMode( accept_mode );
    dialog.setDirectory( path );

    auto all_images_types = make_images_files_filter();
    auto all_images_filter = QString( "Все файлы изображений (%1)" ).arg( all_images_types.join(' ') );
    auto all_files_filter = QString( "Все файлы (*)" );

    dialog.setMimeTypeFilters( _supported_file_types );

    auto filters = dialog.nameFilters();
    filters.append( all_images_filter );
    filters.append( all_files_filter );
    dialog.setNameFilters( filters );

    if( set_all_files_filter )
        dialog.selectNameFilter( all_images_filter );

    dialog.setFileMode( file_mode );

    if( dialog.exec() == QDialog::Accepted )
        return dialog.selectedFiles().first();
    else
        return nullopt;
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

void main_window::scroll_files_tree_view( QString path )
{
    QFileInfo info( path );

    auto index = _file_system_model->index( path );
    if( !index.isValid())
        return;

    if( info.isDir() )
    {
        _ui->files_tree_view->setCurrentIndex( index );
        _ui->files_tree_view->expand( index );

        QTimer::singleShot( 500, [=]()
        {
            auto index = _file_system_model->index( path );
            _ui->files_tree_view->scrollTo( index, QAbstractItemView::PositionAtCenter);
        } );
    }
    else
    {
        _ui->files_tree_view->setCurrentIndex( index );
    }
}
