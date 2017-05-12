#include "current_plot.h"
#include "ui_current_plot.h"
#include "file_worker.h"

Current_Plot::Current_Plot(QVector<QVector<double> > _vec, double _time_end, QWidget *parent) :
    QDialog(parent),
    vec(_vec),
    ui(new Ui::Current_Plot)
{
    ui->setupUi(this);

    QVector<double> time(1000); //Массивы координат точек
    time[0] = 0;
    double h = (_time_end - 0.0)/1000;
    for(int i(1); i < vec[0].size(); i++)
    {
        time[i] = i*h;
    }

    ui->widget->legend->setVisible(true);
    ui->widget->legend->setFont(QFont("Helvetica", 9));
    ui->widget->legend->setRowSpacing(-3);


    //QVector<double> *M = new QVector<double>(vec[0]);

    //рисуем первый график
    {
        ui->widget->clearGraphs();//Если нужно, но очищаем все графики

        //Добавляем один график в widget
        ui->widget->addGraph();
        ui->widget->graph(0)->setData(time, vec[0]);
        ui->widget->graph(0)->setName("Potassium current");



        ui->widget->addGraph();
        ui->widget->graph(1)->setData(time, vec[1]);
        QPen *pen_1 = new QPen;
        pen_1->setColor(QColor(Qt::red));
        ui->widget->graph(1)->setPen(*pen_1);
        ui->widget->graph(1)->setName("Sodium current");


        ui->widget->addGraph();
        ui->widget->graph(2)->setData(time, vec[2]);
        QPen *pen_2 = new QPen;
        pen_2->setColor(QColor(Qt::green));
        ui->widget->graph(2)->setPen(*pen_2);
        ui->widget->graph(2)->setName("Leak current");


        //суммарный ток
        QVector<double> *vec_curr_sum = new QVector<double>;
        for(int i(0); i < vec[0].size(); ++i)
        {
            vec_curr_sum->push_back(vec[0][i]+vec[1][i]+vec[2][i]);
        }
        ui->widget->addGraph();
        ui->widget->graph(3)->setData(time, *vec_curr_sum);
        QPen *pen_3 = new QPen;
        pen_2->setColor(QColor(Qt::black));
        ui->widget->graph(3)->setPen(*pen_3);
        ui->widget->graph(3)->setName("Sum current");


        //Подписываем оси Ox и Oy
        ui->widget->xAxis->setLabel("time [mS]");
        ui->widget->yAxis->setLabel("Current [mA]");

        //Установим область, которая будет показываться на графике
        ui->widget->xAxis->setRange(0, _time_end);//Для оси Ox

        //Для показа границ по оси Oy сложнее, так как надо по правильному
        //вычислить минимальное и максимальное значение в векторах

        double minY = -1000, maxY = 1000;
        ui->widget->yAxis->setRange(minY, maxY);//Для оси Oy

        //И перерисуем график на нашем widget
        ui->widget->replot();
    }




}

Current_Plot::~Current_Plot()
{
    delete ui;
}


