#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HH_model_class.h"
#include <QVector>
#include <QString>
#include <QLineEdit>
#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QIODevice>
#include "file_worker.h"
#include "current_plot.h"
#include "help_browser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionSave_2_triggered();

    void on_actionPlot_current_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    File_Worker *fileWorker;
    QVector<QVector<double> > *result_current_for_plot_all;
    double _time_end;





public slots:
    void take_string_to_status_bar(QString str);
    void save_graphic_1(QString str);
    void save_graphic_2(QString str);

};

#endif // MAINWINDOW_H
