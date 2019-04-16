#pragma once

#include "fft_processor.h"
#include "blob_detector.h"
#include "graphics_view.h"
#include "processing_statisctics_model.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileSystemModel>
#include <QSettings>

namespace Ui
{
	class main_window;
}

class main_window: public QMainWindow
{
	Q_OBJECT

public:
    explicit main_window( dh::fft_processor*,
                          dh::blob_detector*,
						  QWidget *parent = nullptr );
    virtual ~main_window() override;

signals:
    void update_statistics_model( const dh::fft_processing_statistics& );
    void update_statistics_model( const dh::blob_detection_statistics& );

public slots:
    void image_processed( const QImage& );
    void statistics_ready( const dh::fft_processing_statistics& );
    void statistics_ready( const dh::blob_detection_statistics& );

private slots:
	void on_open_image_action_triggered();
    void on_files_tree_view_activated( const QModelIndex& );

private:
    QStringList make_images_files_filter();
    void scroll_files_tree_view( QString path );

private:
    const QStringList _supported_file_types = { "image/png",
                                                "image/jpeg",
                                                "image/bmp" };

    dh::fft_processor* _fft_processor;
    dh::blob_detector* _blob_detector;

	Ui::main_window* _ui;

    QGraphicsScene* _scene;
    QGraphicsPixmapItem* _scene_item;

    QFileSystemModel* _file_system_model;

    dh::processing_statisctics_model* _processing_statistics_model;

    dh::graphics_view* _graphics_view;

    QSettings* _settings;
    const QString _settings_working_path_key;
};
