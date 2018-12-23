#pragma once

#include "fft_viewer_core.h"

#include <QMainWindow>

namespace Ui
{
	class main_window;
}

class main_window : public QMainWindow
{
	Q_OBJECT

public:
	explicit main_window( fft_viewer_core*,
						  QWidget *parent = nullptr );
	~main_window();

private slots:
	void on_open_image_action_triggered();

private:
	fft_viewer_core* _core;
	Ui::main_window* _ui;
};
