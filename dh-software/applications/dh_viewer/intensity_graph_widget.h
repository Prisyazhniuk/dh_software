#pragma once

#include <QChartView>

QT_CHARTS_USE_NAMESPACE

namespace dh
{
    class intensity_graph_widget : public QChartView
    {
        Q_OBJECT

    public:
        explicit intensity_graph_widget( QWidget* parent = nullptr );

    public slots:
        void plot( const QImage&, const QList<QPoint>& );
        void clean();

    private:
        void show_context_menu( const QPoint& );
        void context_menu_save_action();

        bool empty();

        QString export_to_csv();

    private:
        QChart* _chart;
    };
}
