#pragma once

#include "hologram_processor.h"
#include "graphics_view.h"
#include "processing_settings_model.h"
#include "processing_statistics_model.h"
#include "processing_result_type.h"
#include "intensity_graph_model.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTableView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QSettings>
#include <QTabBar>

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
        void image_shown( const QImage& );

    public slots:
        void processed( const processing_results& );
        void error_notified( const QString& );
        void statistics_ready( const processing_statistics& );

    private slots:
        void on_open_image_action_triggered();
        void on_save_image_action_triggered();

        void input_file_selected( const QModelIndex& );
        void settings_changed( const QModelIndex &topLeft, const QModelIndex &bottomRight,
                               const QVector<int> &roles = QVector<int>() );

        void result_tab_selected( int id );

    private:
        std::optional<QString> execute_file_dialog( const QString& caption, const QString& path,
                                                    QFileDialog::AcceptMode accept_mode,
                                                    QFileDialog::FileMode file_mode,
                                                    bool set_all_files_filter );
        QStringList make_images_files_filter();
        void scroll_files_tree_view( QString path );

        void show_results( processing_result_type );

    private:
        const QStringList _supported_file_types = { "image/png",
                                                    "image/jpeg",
                                                    "image/bmp" };

        hologram_processor* _hologram_processor;
        processing_results _processing_results;

        cv::Mat _source_image;

        Ui::main_window* _ui;

        QGraphicsScene* _scene;
        QGraphicsPixmapItem* _scene_item;
        QTabBar* _results_tab_bar;
        std::mutex _scene_item_mutex;

        QFileSystemModel* _file_system_model;
        QTreeView* _file_system_view;

        processing_settings_model* _processing_settings_model;
        intensity_graph_model* _intensity_graph_model;
        QTableView* _intensity_graph_view;
        processing_statistics_model* _processing_statistics_model;

        graphics_view* _graphics_view;

        QSettings* _settings;
        const QString _settings_working_path_key;
        const QString _settings_save_path_key;
    };
}
