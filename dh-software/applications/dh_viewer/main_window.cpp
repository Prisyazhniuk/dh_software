#include "main_window.h"
#include "ui_main_window.h"
#include "float_item_delegate.h"
#include "image_loader.h"
#include "intensity_graph_widget.h"

#include "exceptions/argument_exception.h"

#include <QGuiApplication>
#include <QScreen>
#include <QHeaderView>
#include <QFileDialog>
#include <QMimeDatabase>
#include <QTimer>
#include <QMessageBox>
#include <QDockWidget>

using namespace std;
using namespace cv;

namespace dh
{
    main_window::main_window( hologram_processor* hologram_processor,
                              QWidget *parent )
        : QMainWindow( parent )
        , _hologram_processor( hologram_processor )
        , _processing_results( {} )
        , _ui( new Ui::main_window )
        , _settings_working_path_key( "working_path" )
        , _settings_save_path_key( "save_path" )
    {
        if( !hologram_processor )
            throw argument_exception( "hologram_processor is null", get_exception_source() );

        connect( _hologram_processor, &hologram_processor::processed,
                 this, &main_window::processed );

        connect( _hologram_processor, &hologram_processor::error,
                 this, &main_window::error_notified );

        connect( _hologram_processor, &hologram_processor::statistics_ready,
                 this, &main_window::statistics_ready );

        _settings = new QSettings( "dh", "dh_viewer", this );

        _ui->setupUi( this );

        _scene = new QGraphicsScene( this );
        _scene_item = _scene->addPixmap( QPixmap() );

        _results_tab_bar = new QTabBar( this );
        _results_tab_bar->setExpanding( false );
        _results_tab_bar->addTab( "Оригинал" );
        _results_tab_bar->addTab( "Фаза" );
        _results_tab_bar->addTab( "Амплитуда" );
        _results_tab_bar->addTab( "Интенсивность" );
        _results_tab_bar->addTab( "Реальная часть" );
        _results_tab_bar->addTab( "Мнимая часть" );
        _results_tab_bar->setTabData( 0, qVariantFromValue(processing_result_type::original) );
        _results_tab_bar->setTabData( 1, qVariantFromValue(processing_result_type::phase) );
        _results_tab_bar->setTabData( 2, qVariantFromValue(processing_result_type::amplitude) );
        _results_tab_bar->setTabData( 3, qVariantFromValue(processing_result_type::intensity) );
        _results_tab_bar->setTabData( 4, qVariantFromValue(processing_result_type::real) );
        _results_tab_bar->setTabData( 5, qVariantFromValue(processing_result_type::imaginary) );

        connect( _results_tab_bar, &QTabBar::currentChanged, this, &main_window::result_tab_selected );

        _ui->graphics_view_layout->addWidget( _results_tab_bar );

        _graphics_view = new graphics_view( this );
        _graphics_view->set_scene( _scene );
        _ui->graphics_view_layout->addWidget( _graphics_view );

        {
            auto images_files_filter = make_images_files_filter();
            _file_system_model = new QFileSystemModel( this );
            _file_system_model->setRootPath( "" );
            _file_system_model->setNameFilters( images_files_filter );
            _file_system_model->setNameFilterDisables( false );

            auto dock = new QDockWidget( "Файлы", this );
            dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

            _file_system_view = new QTreeView( dock );
            auto view = _file_system_view;
            view->setModel( _file_system_model );
            for( int i = 1; i < _file_system_model->columnCount(); i++ )
                view->hideColumn( i );
            view->header()->setSectionResizeMode( QHeaderView::ResizeToContents );
            view->setHeaderHidden( true );

            dock->setWidget( view );
            addDockWidget( Qt::LeftDockWidgetArea, dock );

            _ui->view_menu->addAction( dock->toggleViewAction() );

            auto working_path = _settings->value( _settings_working_path_key, "" ).toString();
            scroll_files_tree_view( working_path );

            connect( _file_system_view, &QTreeView::activated,
                     this, &main_window::input_file_selected );
        }

        auto settings = processing_settings
        {
            .lambda_mm = 0.0006328f,
            .sensor_width_mm = 4.24f,
            .sensor_height_mm = 2.39f,
            .distance_mm = 23.2f,
            .theta_rad = 0
        };

        {
            _processing_settings_model = new processing_settings_model( settings, this );

            auto dock = new QDockWidget( "Параметры", this );
            dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

            auto view = new QTableView( dock );
            view->setModel( _processing_settings_model );
            view->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
            view->horizontalHeader()->setStretchLastSection( true );
            view->horizontalHeader()->hide();
            view->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
            view->verticalHeader()->hide();
            view->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
            view->setSizeAdjustPolicy( QAbstractScrollArea::AdjustToContents );
            view->setItemDelegateForRow( 0, new float_item_delegate( 8, 0.0000001, view ) ); // lambda_mm
            view->setItemDelegateForRow( 1, new float_item_delegate( 3, 0.01, view ) ); // sensor_width_mm
            view->setItemDelegateForRow( 2, new float_item_delegate( 3, 0.01, view ) ); // sensor_height_mm
            view->setItemDelegateForRow( 3, new float_item_delegate( 2, 0.1, view ) ); // distance_mm
            view->setItemDelegateForRow( 4, new float_item_delegate( 3, 0.01, view ) ); // theta_rad

            dock->setWidget( view );
            addDockWidget( Qt::LeftDockWidgetArea, dock );

            _ui->view_menu->addAction( dock->toggleViewAction() );

            connect( _processing_settings_model, &processing_settings_model::dataChanged,
                     this, &main_window::settings_changed );
        }

        {
            _intensity_graph_model = new intensity_graph_model( _scene, _scene_item, this );

            auto dock = new QDockWidget( "График интенсивности", this );
            dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

            auto dock_layout = new QVBoxLayout();
            dock_layout->setMargin( 0 );

            auto central_widget = new QWidget();
            central_widget->setLayout( dock_layout );
            central_widget->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );

            _intensity_graph_view = new QTableView();
            _intensity_graph_view->setModel( _intensity_graph_model );
            _intensity_graph_view->setSpan( 3, 0, 1, 2 );
            _intensity_graph_view->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
            _intensity_graph_view->horizontalHeader()->setStretchLastSection( true );
            _intensity_graph_view->horizontalHeader()->hide();
            _intensity_graph_view->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
            _intensity_graph_view->verticalHeader()->hide();
            _intensity_graph_view->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
            _intensity_graph_view->setSizeAdjustPolicy( QAbstractScrollArea::AdjustToContents );
            _intensity_graph_view->setEnabled( false );
            dock_layout->addWidget( _intensity_graph_view );

            auto intensity_graph = new intensity_graph_widget( central_widget );
            intensity_graph->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
            intensity_graph->setSizeAdjustPolicy( QAbstractScrollArea::AdjustToContents );
            dock_layout->addWidget( intensity_graph );

            dock->setWidget( central_widget );
            addDockWidget( Qt::LeftDockWidgetArea, dock );

            _ui->view_menu->addAction( dock->toggleViewAction() );

            QObject::connect( this, &main_window::image_shown,
                              _intensity_graph_model, &intensity_graph_model::image_change );

            QObject::connect( _intensity_graph_model, &intensity_graph_model::plot,
                              intensity_graph, &intensity_graph_widget::plot );

            QObject::connect( _intensity_graph_model, &intensity_graph_model::disabled,
                              intensity_graph, &intensity_graph_widget::clean );
        }

        {
            _processing_statistics_model = new processing_statistics_model( this );

            auto dock = new QDockWidget( "Статистика", this );
            dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

            auto view = new QTableView( dock );
            view->setModel( _processing_statistics_model );
            view->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
            view->horizontalHeader()->setStretchLastSection( true );
            view->horizontalHeader()->hide();
            view->verticalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
            view->verticalHeader()->hide();
            view->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum );
            view->setSizeAdjustPolicy( QAbstractScrollArea::AdjustToContents );

            dock->setWidget( view );
            addDockWidget( Qt::LeftDockWidgetArea, dock );

            _ui->view_menu->addAction( dock->toggleViewAction() );

            connect( this, &main_window::update_statistics_model,
                     _processing_statistics_model, &processing_statistics_model::update_statistics );
        }

        resize( 800, 600 );
        showMaximized();
    }

    main_window::~main_window()
    {
        _hologram_processor->stop();
        delete _ui;
    }

    void main_window::processed( const processing_results& results )
    {
        _processing_results = results;

        auto current_tab = _results_tab_bar->currentIndex();
        auto result_type = _results_tab_bar->tabData( current_tab ).value<processing_result_type>();

        show_results( result_type );
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

    void main_window::input_file_selected( const QModelIndex& index )
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

    void main_window::result_tab_selected( int id )
    {
        auto result_type = _results_tab_bar->tabData( id ).value<processing_result_type>();

        show_results( result_type );
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
            _file_system_view->setCurrentIndex( index );
            _file_system_view->expand( index );

            QTimer::singleShot( 500, [=]()
            {
                auto index = _file_system_model->index( path );
                _file_system_view->scrollTo( index, QAbstractItemView::PositionAtCenter);
            } );
        }
        else
        {
            _file_system_view->setCurrentIndex( index );
        }
    }

    void main_window::show_results( processing_result_type type )
    {
        optional<Mat> mat = nullopt;

        switch( type )
        {
            case processing_result_type::original:
                mat = _processing_results.original;
                break;

            case processing_result_type::phase:
                mat = _processing_results.phase;
                break;

            case processing_result_type::amplitude:
                mat = _processing_results.amplitude;
                break;

            case processing_result_type::intensity:
                mat = _processing_results.intensity;
                break;

            case processing_result_type::real:
                mat = _processing_results.real;
                break;

            case processing_result_type::imaginary:
                mat = _processing_results.imaginary;
                break;
        }

        if( !mat )
        {
            mat = Mat();
        }

        auto& m = mat.value();
        auto image = QImage( m.data, m.cols, m.rows, static_cast<int>( m.step ), QImage::Format_Grayscale8 );
        auto pixmap = QPixmap::fromImage( image );

        lock_guard<mutex> lock( _scene_item_mutex );

        if( _scene_item->pixmap().rect() != pixmap.rect() )
            _scene->setSceneRect( pixmap.rect() );

        _scene_item->setPixmap( pixmap );

        emit image_shown( image );

        _intensity_graph_view->setEnabled( true );
    }
}
