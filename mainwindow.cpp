//
// Created by Des Caldnd on 11/14/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <expression_process.h>


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Graph");

    connect(ui->box_x_max, &QDoubleSpinBox::valueChanged, this, &MainWindow::changed_x_max);
    connect(ui->box_x_min, &QDoubleSpinBox::valueChanged, this, &MainWindow::changed_x_min);
    connect(ui->box_y_max, &QDoubleSpinBox::valueChanged, this, &MainWindow::changed_y_max);
    connect(ui->box_y_min, &QDoubleSpinBox::valueChanged, this, &MainWindow::changed_y_min);
    connect(ui->box_sections, &QSpinBox::valueChanged, this, &MainWindow::changed_sections);
    connect(ui->line_expression, &QLineEdit::textEdited, this, &MainWindow::prompt_changed);

    ui->widget_graph->xAxis->setRange(x_min, x_max);
    ui->widget_graph->yAxis->setRange(y_min, y_max);
    ui->statusbar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    vector_destroy(&expr_vector);
}

void MainWindow::changed_x_max(double val)
{
    if(val > x_min)
    {
        x_max = val;
        ui->widget_graph->xAxis->setRange(x_min, x_max);
        plot();
    }
}

void MainWindow::changed_x_min(double val)
{
    if(val < x_max)
    {
        x_min = val;
        ui->widget_graph->xAxis->setRange(x_min, x_max);
        plot();
    }
}

void MainWindow::changed_y_max(double val)
{
    if(val > y_min)
    {
        y_max = val;
        ui->widget_graph->yAxis->setRange(y_min, y_max);
        ui->widget_graph->replot();
    }
}

void MainWindow::changed_y_min(double val)
{
    if(val < y_max)
    {
        y_min = val;
        ui->widget_graph->yAxis->setRange(y_min, y_max);
        ui->widget_graph->replot();
    }
}

void MainWindow::changed_sections(int val)
{
    sections = val;
    plot();
}

void MainWindow::plot()
{
    expr_vector = expression_to_postfix(prompt.data());
    if (!vector_is_valid(expr_vector))
    {
        clear();
        return;
    }

    prepare_data();
    for (int i = ui->widget_graph->graphCount() - 1; i >= data.size(); --i)
    {
        ui->widget_graph->removeGraph(i);
    }
    for (int i = ui->widget_graph->graphCount(); i < data.size(); ++i)
    {
        ui->widget_graph->addGraph();
    }

    for (int i = 0; i < data.size(); ++i)
    {
        ui->widget_graph->graph(i)->setData(x_data[i], data[i]);
    }

    ui->widget_graph->replot();
}

void MainWindow::prompt_changed(QString string)
{
    prompt = string.toStdString();
    plot();
}

void MainWindow::prepare_data()
{
    double x = x_min;
    int vec_index = 0;
    bool need_new = false;
    data.clear();
    data.push_back(QVector<double>());
    x_data.clear();
    x_data.push_back(QVector<double>());
    double precision = (x_max - x_min) / sections;

    while (x < x_max)
    {
        double val = calculate_expression(expr_vector, x);
        if (val != NAN && val != INFINITY && val != -INFINITY)
        {
            x_data[vec_index].push_back(x);
            data[vec_index].push_back(val);
            need_new = true;
        } else if (need_new)
        {

            data.push_back(QVector<double>());
            x_data.push_back(QVector<double>());
            need_new = false;
            ++vec_index;
        }
        x += precision;
    }
    if (need_new)
    {
        double val = calculate_expression(expr_vector, x);
        if (val != NAN && val != INFINITY && val != -INFINITY)
        {
            x_data[vec_index].push_back(x);
            data[vec_index].push_back(val);
        }
    }
}

void MainWindow::clear()
{
        ui->widget_graph->clearGraphs();
        ui->widget_graph->replot();
}
