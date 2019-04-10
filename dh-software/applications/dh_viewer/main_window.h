#pragma once

#include "fft_processor.h"
#include "graphics_view.h"
#include "processing_statisctics_model.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui
{
	class main_window;
}

class main_window: public QMainWindow
{
	Q_OBJECT

public:
    explicit main_window( dh::fft_processor*,
						  QWidget *parent = nullptr );
    virtual ~main_window() override;

signals:
    void update_statistics_model( const dh::fft_processing_statistics& );

public slots:
    void image_processed( const QImage& );
    void statistics_ready( const dh::fft_processing_statistics& );

private slots:
	void on_open_image_action_triggered();

private:
    dh::fft_processor* _fft_processor;
	Ui::main_window* _ui;

    QGraphicsScene* _scene;
    QGraphicsPixmapItem* _scene_item;

    dh::processing_statisctics_model* _processing_statistics_model;

    dh::graphics_view* _graphics_view;
};
