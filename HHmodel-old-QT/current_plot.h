#ifndef CURRENT_PLOT_H
#define CURRENT_PLOT_H

#include <QDialog>
#include <QtWidgets>
#include <QVector>

namespace Ui {
class Current_Plot;
}

class Current_Plot : public QDialog
{
    Q_OBJECT

public:
    explicit Current_Plot(QVector<QVector<double> >, double , QWidget *parent = 0);
    ~Current_Plot();

private:
    Ui::Current_Plot *ui;
    QVector<QVector<double> > vec;

};

#endif // CURRENT_PLOT_H
