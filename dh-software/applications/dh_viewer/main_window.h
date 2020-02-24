#pragma once

#include "hologram_processor.h"
#include "graphics_view.h"
#include "processing_settings_model.h"
#include "processing_statistics_model.h"
#include "intensity_graph_model.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QSettings>

namespace Ui
{
	class main_window;
}

namespace dh
{
    class main_window: public QMainWindow
    {
        Q_OBJECT

    public:
        explicit main_window( hologram_processor*, QWidget *parent = nullptr );
        virtual ~main_window() override;

    signals:
        void update_statistics_model( const processing_statistics& );

    public slots:
        void image_processed( const QImage& );
        void error_notified( const QString& );
        void statistics_ready( const processing_statistics& );

    private slots:
        void on_open_image_action_triggered();
        void on_save_image_action_triggered();

        void on_files_tree_view_activated( const QModelIndex& );
        void settings_changed( const QModelIndex &topLeft, const QModelIndex &bottomRight,
                               const QVector<int> &roles = QVector<int>() );

    private:
        std::optional<QString> execute_file_dialog( const QString& caption, const QString& path,
                                                    QFileDialog::AcceptMode accept_mode,
                                                    QFileDialog::FileMode file_mode,
                                                    bool set_all_files_filter );
        QStringList make_images_files_filter();
        void scroll_files_tree_view( QString path );

    private:
        const QStringList _supported_file_types = { "image/png",
                                                    "image/jpeg",
                                                    "image/bmp" };

        hologram_processor* _hologram_processor;

        cv::Mat _source_image;

        Ui::main_window* _ui;

        QGraphicsScene* _scene;
        QGraphicsPixmapItem* _scene_item;
        std::mutex _scene_item_mutex;

        QFileSystemModel* _file_system_model;

        processing_settings_model* _processing_settings_model;
        intensity_graph_model* _intensity_graph_model;
        processing_statistics_model* _processing_statistics_model;

        graphics_view* _graphics_view;

        QSettings* _settings;
        const QString _settings_working_path_key;
        const QString _settings_save_path_key;
    };
}
