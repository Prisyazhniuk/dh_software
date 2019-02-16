#pragma once

#include "fft_viewer_core.h"
#include "graphics_view.h"

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
    explicit main_window( dh::fft_viewer_core*,
						  QWidget *parent = nullptr );
    virtual ~main_window() override;

public slots:
    void show_image( const QImage& );

private slots:
	void on_open_image_action_triggered();

private:
    dh::fft_viewer_core* _core;
	Ui::main_window* _ui;

    QGraphicsScene* _scene;
    QGraphicsPixmapItem* _scene_item;

    dh::graphics_view* _graphics_view;
};
