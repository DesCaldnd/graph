//
// Created by Des Caldnd on 11/14/2023.
//

#ifndef GRAPH_MAINWINDOW_H
#define GRAPH_MAINWINDOW_H

#include <QMainWindow>
#include <structs.h>
#include <string>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:

    void changed_x_max(double val);
    void changed_x_min(double val);
    void changed_y_max(double val);
    void changed_y_min(double val);
    void changed_sections(int val);
    void prompt_changed(QString string);

private:
    vector expr_vector;
    double x_max = 1, x_min = -1, y_max = 1, y_min = -1;
    int sections = 200;
    std::string prompt;
    Ui::MainWindow *ui;
    QVector<QVector<double>> data, x_data;

    void plot();
    void prepare_data();
    void clear();
};


#endif //GRAPH_MAINWINDOW_H
