#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(), this->geometry().height());



    fileWorker = new File_Worker();


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->actionSave_2->setIcon(QIcon(":/save/Treetog-I-Floppy-Small.ico"));
    ui->actionSave_2->setToolTip("<h3>Сохранить графики в файл</h3>");

    ui->actionPlot_current->setIcon(QIcon(":/cur/076-512.png"));
    ui->actionPlot_current->setToolTip("<h3>Построить графики токов</h3>");

    ui->actionAbout->setIcon(QIcon(":/about/about.png"));
    ui->actionAbout->setToolTip("<h3>О программе!</h3>");


    ui->actionPlot_current->setDisabled(true);





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    /*
    double Cm = 1;

    double Gk = 36;
    double Gna = 120;
    double Gl = 0.3;

    double Vk = -77;
    double Vna = 50;
    double Vl = -54.387;
    */
    //включение/выключение каналов
    std::vector<int> channel_on_off;
    if(ui->checkBox->checkState() == Qt::Checked){
        channel_on_off.push_back(1);
    }else{
        channel_on_off.push_back(0);
    }
    if(ui->checkBox_2->checkState() == Qt::Checked){
        channel_on_off.push_back(1);
    }else{
        channel_on_off.push_back(0);
    }
    if(ui->checkBox_3->checkState() == Qt::Checked){
        channel_on_off.push_back(1);
    }else{
        channel_on_off.push_back(0);
    }

    //передача параметров через Эдиты

    double Cm = ui->lineEdit->text().toDouble();
    double Stimul_voltage = ui->lineEdit_2->text().toDouble();

    double Gk = ui->lineEdit_3->text().toDouble();
    double Gna = ui->lineEdit_5->text().toDouble();
    double Gl = ui->lineEdit_7->text().toDouble();

    double Vk = ui->lineEdit_4->text().toDouble();
    double Vna = ui->lineEdit_6->text().toDouble();
    double Vl = ui->lineEdit_8->text().toDouble();

    QRegExp exp("[0-3]{1}[0-9]{1}");
    ui->lineEdit_10->setValidator(new QRegExpValidator(exp, this));
    _time_end = ui->lineEdit_10->text().toDouble();

    double time_stimul_duration = ui->lineEdit_9->text().toDouble();

    double time_stimulation_start = ui->lineEdit_11->text().toDouble();



    //constant parameter for solver
    std::vector<double> param;

    param.push_back(Cm);
    param.push_back(Gk);
    param.push_back(Vk);
    param.push_back(Gna);
    param.push_back(Vna);
    param.push_back(Gl);
    param.push_back(Vl);
    param.push_back(Stimul_voltage);
    param.push_back(time_stimul_duration);
    param.push_back(time_stimulation_start);





    //start point for solver
    std::vector<double> Y;
    Y.push_back(-64.6489); //start voltage
    Y.push_back(0.3347); //start n
    Y.push_back(0.0677); //start m
    Y.push_back(0.5864); //start h

    //решаем уравнение(создаём объект класса(солвер в конструкторе))
    std::vector<double> _time;
    _time.push_back(0);
    _time.push_back(_time_end);


    HH_model_class *HH_model = new HH_model_class(param, Y, _time, channel_on_off);

    // результаты решения уравненй
    QVector<double> *result_vi = new QVector<double>(QVector<double>::fromStdVector(HH_model->return_result_vi()));
    QVector<double> *result_n = new QVector<double>(QVector<double>::fromStdVector(HH_model->return_result_n()));
    QVector<double> *result_m = new QVector<double>(QVector<double>::fromStdVector(HH_model->return_result_m()));
    QVector<double> *result_h = new QVector<double>(QVector<double>::fromStdVector(HH_model->return_result_h()));

    //Получение результатов для построение токов
    QVector<double> *result_current_K = new QVector<double>(QVector<double>::fromStdVector(HH_model->return_result_current_K()));
    QVector<double> *result_current_Na = new QVector<double>(QVector<double>::fromStdVector(HH_model->return_result_current_Na()));
    QVector<double> *result_current_L = new QVector<double>(QVector<double>::fromStdVector(HH_model->return_result_current_L()));

    QVector<QVector<double> > *result_current_all = new QVector<QVector<double> >;
    result_current_all->push_back(*result_current_K);
    result_current_all->push_back(*result_current_Na);
    result_current_all->push_back(*result_current_L);

    //fileWorker->save_current_to_file(*result_current_all);
    ui->actionPlot_current->setEnabled(true);

    result_current_for_plot_all = result_current_all;



    //сохранение значений решения ODE и параметров в файлы
    QVector<QVector <double> > *result_all = new QVector<QVector <double> >;
    result_all->push_back(*result_vi);
    result_all->push_back(*result_n);
    result_all->push_back(*result_m);
    result_all->push_back(*result_h);

    fileWorker->save_data_to_text(*result_all);
    fileWorker->save_parameters_to_text(QVector<double>::fromStdVector(param));






    //построение графиков


    double time_start = _time[0]; //Начало интервала, где рисуем график по оси Ox
    double time_end =  _time[1]; //Конец интервала, где рисуем график по оси Ox
    int N = 1000;
    double h = (time_end-time_start)/N; //Шаг, с которым будем пробегать по оси Ox

    QVector<double> time(N); //Массивы координат точек
    time[0] = time_start;
    for(int i(0); i < result_vi->size(); i++)
    {
        time[i] = i*h;
    }


    //----------------------------------------------------------
    //находим максимумы
    QVector<double> *maximum_vi = new QVector<double>;
    QVector<double> *maximum_time_vi = new QVector<double>;


    for(int i(1); i < result_vi->size()-1; i++)
    {
        if((*result_vi)[i-1] <= (*result_vi)[i] && (*result_vi)[i] >= (*result_vi)[i+1])
        {
            maximum_vi->push_back((*result_vi)[i]);
            maximum_time_vi->push_back(time[i]);
        }
    }

    //строим таблицу для показа значений
    ui->tableWidget->setRowCount(maximum_vi->size());
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "time(mS)" << "potentian(mV)");

    //добавляем значения в tableWidget
    for(int i(0); i < ui->tableWidget->rowCount(); i++){
        for(int j(0); j < ui->tableWidget->columnCount(); j++){
            if(j == 0){
                //в первую колонку время
                QTableWidgetItem *itm = new QTableWidgetItem(tr("%1").arg((*maximum_time_vi)[i]));
                ui->tableWidget->setItem(i,j,itm);
            }else{
                //во вторую колонку потенциал
                QTableWidgetItem *itm = new QTableWidgetItem(tr("%1").arg((*maximum_vi)[i]));
                ui->tableWidget->setItem(i,j,itm);
            }
        }
    }
    //----------------------------------------------------------


    ui->widget->legend->setVisible(true);
    ui->widget_2->legend->setVisible(true);
    ui->widget_3->legend->setVisible(true);

    ui->widget->legend->setFont(QFont("Helvetica", 9));
    ui->widget_2->legend->setFont(QFont("Helvetica", 9));
    ui->widget_3->legend->setFont(QFont("Helvetica", 9));

    ui->widget->legend->setRowSpacing(-3);
    ui->widget_2->legend->setRowSpacing(-3);
    ui->widget_3->legend->setRowSpacing(-3);

    //рисуем первый график
    {
        ui->widget->clearGraphs();//Если нужно, но очищаем все графики

        //Добавляем один график в widget
        ui->widget->addGraph();
        //Говорим, что отрисовать нужно график по нашим двум массивам x и y
        ui->widget->graph(0)->setData(time, *result_vi);
        ui->widget->graph(0)->setName("Potential");



        // рисуем максимумы
        ui->widget->addGraph();
        ui->widget->graph(1)->setData(*maximum_time_vi, *maximum_vi);
        ui->widget->graph(1)->setName("Maximums");


        ui->widget->graph(1)->setPen(QColor(50, 50, 50, 255));//задаем цвет точки
        ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);//убираем линии
        //формируем вид точек
        ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));



        //Подписываем оси Ox и Oy
        ui->widget->xAxis->setLabel("time [mS]");
        ui->widget->yAxis->setLabel("Potential [mV]");

        //Установим область, которая будет показываться на графике
        ui->widget->xAxis->setRange(time_start, time_end);//Для оси Ox

        //Для показа границ по оси Oy сложнее, так как надо по правильному
        //вычислить минимальное и максимальное значение в векторах

        double minY = -100, maxY = 100;
        ui->widget->yAxis->setRange(minY, maxY);//Для оси Oy

        //И перерисуем график на нашем widget
        ui->widget->replot();
    }
    //рисуем второй графие
    {
        ui->widget_2->clearGraphs();//Если нужно, но очищаем все графики

        //Добавляем один график в widget
        ui->widget_2->addGraph();
        ui->widget_2->graph(0)->setData(time, *result_n);
        ui->widget_2->graph(0)->setName("n value");

        ui->widget_2->addGraph();
        ui->widget_2->graph(1)->setData(time, *result_m);
        QPen *pen_1 = new QPen;
        pen_1->setColor(QColor(Qt::red));
        ui->widget_2->graph(1)->setPen(*pen_1);
        ui->widget_2->graph(1)->setName("m value");


        ui->widget_2->addGraph();
        ui->widget_2->graph(2)->setData(time, *result_h);
        QPen *pen_2 = new QPen;
        pen_2->setColor(QColor(Qt::green));
        ui->widget_2->graph(2)->setPen(*pen_2);
        ui->widget_2->graph(2)->setName("h value");


        //Подписываем оси Ox и Oy
        ui->widget_2->xAxis->setLabel("time [mS]");
        ui->widget_2->yAxis->setLabel("Propability [dimensionless]");

        //Установим область, которая будет показываться на графике
        ui->widget_2->xAxis->setRange(time_start, time_end);//Для оси Ox

        //Для показа границ по оси Oy сложнее, так как надо по правильному
        //вычислить минимальное и максимальное значение в векторах
        ui->widget_2->yAxis->setRange(0, 1);//Для оси Oy

        //И перерисуем график на нашем widget
        ui->widget_2->replot();
    }
    //рисуем третий графие ступеньку
    {


        QVector<double> *amplitude_stimul_duration = new QVector<double>;

        for(int i(0); i < time.size(); ++i)
        {
            if( (time[i] >= time_stimul_duration) || (time[i] <= time_stimulation_start) )
            {
                amplitude_stimul_duration->push_back(0.0);
            }else
            {
                amplitude_stimul_duration->push_back( Stimul_voltage );
            }
        }


        ui->widget_3->clearGraphs();//Если нужно, но очищаем все графики

        //Добавляем один график в widget
        ui->widget_3->addGraph();
        ui->widget_3->graph(0)->setData(time, *amplitude_stimul_duration);
        ui->widget_3->graph(0)->setName("Stimulate current");

        //Подписываем оси Ox и Oy
        ui->widget_3->xAxis->setLabel("time [mS]");
        ui->widget_3->yAxis->setLabel("Voltage [mV]");

        //Установим область, которая будет показываться на графике
        ui->widget_3->xAxis->setRange(time_start, time_end);//Для оси Ox

        //Для показа границ по оси Oy сложнее, так как надо по правильному
        //вычислить минимальное и максимальное значение в векторах
        ui->widget_3->yAxis->setRange(-5, Stimul_voltage+5);//Для оси Oy

        //И перерисуем график на нашем widget
        ui->widget_3->replot();
    }

}

void MainWindow::on_pushButton_2_clicked()
{

    double Cm = 1.0;

    double Gk = 36.0;
    double Gna = 120.0;
    double Gl = 0.3;

    double Vk = -77.0;
    double Vna = 50.0;
    double Vl = -54.387;
    double Stimul_voltage = 10.0;

    double time_end = 10.0;
    double time_stimul_duration = 2.0;

    double time_stimulation_start = 1.0;

    ui->lineEdit->setText(QString::number(Cm));
    ui->lineEdit_2->setText(QString::number(Stimul_voltage));

    ui->lineEdit_3->setText(QString::number(Gk));
    ui->lineEdit_5->setText(QString::number(Gna));
    ui->lineEdit_7->setText(QString::number(Gl));

    ui->lineEdit_4->setText(QString::number(Vk));
    ui->lineEdit_6->setText(QString::number(Vna));
    ui->lineEdit_8->setText(QString::number(Vl));


    ui->checkBox->setChecked(Qt::Checked);
    ui->checkBox_2->setChecked(Qt::Checked);
    ui->checkBox_3->setChecked(Qt::Checked);

    ui->lineEdit_10->setText(QString::number(time_end));
    ui->lineEdit_9->setText(QString::number(time_stimul_duration));
    ui->lineEdit_11->setText(QString::number(time_stimulation_start));


}

void MainWindow::on_actionSave_2_triggered()
{

    //Сохраняем картинки в файл file_i.png

    //File_Worker *fileWorker = new File_Worker();
    QObject::connect( fileWorker, SIGNAL(sendString(QString)), this, SLOT(take_string_to_status_bar(QString)) );
    QObject::connect( fileWorker, SIGNAL(save_graph_1(QString)), this, SLOT(save_graphic_1(QString)) );
    QObject::connect( fileWorker, SIGNAL(save_graph_2(QString)), this, SLOT(save_graphic_2(QString)) );
    fileWorker->load_iterator_from_file();
    fileWorker->save_iterator_to_file();
    fileWorker->save_potential_to_file();
    fileWorker->save_propobility_to_file();




}

//ответ от класса файл о состоянии сохранения
void MainWindow::take_string_to_status_bar(QString str)
{
    ui->statusBar->showMessage(str);
}

//слоты принимающие имя файла для сохранения
void MainWindow::save_graphic_1(QString str)
{
    ui->widget->savePng( str,  1024, 768, 1.0 );

}
void MainWindow::save_graphic_2(QString str)
{
    ui->widget_2->savePng( str,  1024, 768, 1.0 );

}

void MainWindow::on_actionPlot_current_triggered()
{
    Current_Plot *wnd = new Current_Plot(*result_current_for_plot_all,_time_end, this);
    wnd->show();
    ui->actionPlot_current->setDisabled(true);
}

void MainWindow::on_actionAbout_triggered()
{
    Help_Browser  *helpBrowser = new Help_Browser(":/help/doc/", "index.htm");
    helpBrowser->resize(800, 600);
    helpBrowser->setFixedSize(helpBrowser->geometry().width(), helpBrowser->geometry().height());
    helpBrowser->show();
}
