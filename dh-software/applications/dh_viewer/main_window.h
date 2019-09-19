#pragma once

#include "hologram_processor.h"
#include "graphics_view.h"
#include "processing_settings_model.h"
#include "processing_statistics_model.h"

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
    explicit main_window( dh::hologram_processor*, QWidget *parent = nullptr );
    virtual ~main_window() override;

signals:
    void update_statistics_model( const dh::processing_statistics& );

public slots:
    void image_processed( const QImage& );
    void error_notified( const QString& );
    void statistics_ready( const dh::processing_statistics& );

private slots:
	void on_open_image_action_triggered();
    void on_files_tree_view_activated( const QModelIndex& );
    void settings_changed( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>() );

private:
    QStringList make_images_files_filter();
    void scroll_files_tree_view( QString path );

private:
    const QStringList _supported_file_types = { "image/png",
                                                "image/jpeg",
                                                "image/bmp" };

    dh::hologram_processor* _hologram_processor;

    cv::Mat _hologram;

	Ui::main_window* _ui;

    QGraphicsScene* _scene;
    QGraphicsPixmapItem* _scene_item;

    QFileSystemModel* _file_system_model;

    dh::processing_settings_model* _processing_settings_model;
    dh::processing_statistics_model* _processing_statistics_model;

    dh::graphics_view* _graphics_view;

    QSettings* _settings;
    const QString _settings_working_path_key;
};
