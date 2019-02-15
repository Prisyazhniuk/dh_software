#pragma once

#include "fft_viewer_core.h"
#include "graphics_view.h"

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui
{
	class main_window;
}

class main_window : public QMainWindow
{
	Q_OBJECT

public:
    explicit main_window( dh::fft_viewer_core*,
						  QWidget *parent = nullptr );
	~main_window();

public slots:
    void show_image( const QPixmap& );

private slots:
	void on_open_image_action_triggered();

private:
    dh::fft_viewer_core* _core;
	Ui::main_window* _ui;

    QGraphicsScene* _scene;
    dh::graphics_view* _graphics_view;
};
