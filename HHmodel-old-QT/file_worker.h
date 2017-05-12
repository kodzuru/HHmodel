#ifndef FILE_WORKER_H
#define FILE_WORKER_H

#include <QFile>
#include <QString>
#include <QtWidgets>
#include <QMessageBox>
#include <QObject>

class File_Worker : public QObject
{
    Q_OBJECT
public:
    File_Worker(QObject *parent = 0);
private:
    int iterator;
    QString outputDir;
    QString fileName;
public:
    void load_iterator_from_file();
    void save_iterator_to_file();
    void save_potential_to_file();
    void save_propobility_to_file();
    void save_data_to_text(QVector<QVector <double> > &);
    void save_parameters_to_text(QVector<double>);
    void save_current_to_file(QVector<QVector<double> > &);
signals:
    void sendString(QString str);
    void save_graph_1(QString str);
    void save_graph_2(QString str);
};

#endif // FILE_WORKER_H
