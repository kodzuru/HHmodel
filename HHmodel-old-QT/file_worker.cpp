#include "file_worker.h"
#include <QMessageBox>

File_Worker::File_Worker(QObject *parent) :
    QObject(parent)

{
    //директория в которую всё сохраняем
    outputDir = QApplication::applicationDirPath() + "/Data";

    //Создаём папку если не существует
    QDir dir(QApplication::applicationDirPath()+"/"+"Data");
    if (!dir.exists()){
        dir.mkdir(".");
    }
    //создаём файл если файла не существует и записываем в него 0
    QFile file(outputDir + "/Info.dat");
    if(!file.exists())
    {
        if(!file.open(QIODevice::WriteOnly | QFile::Text))
        {
            emit sendString("Path not correct !!! ");

        }else{

            emit sendString("File succesfull crealte !!!");

            QTextStream stream(&file);
            stream << 0;
            file.flush();
            file.close();
        }

    }
}

//загружаем счётчик из файла
void File_Worker::load_iterator_from_file()
{
    QFile dataFile(outputDir + "/Info.dat");
    if(!dataFile.open(QIODevice::ReadOnly | QFile::Text))
    {


    }else{

        QTextStream stream(&dataFile);
        QString buffer = stream.readAll();

        iterator = buffer.toInt() + 1;

        dataFile.flush();
        dataFile.close();
    }

}
//сохраняем счётчик в файл
void File_Worker::save_iterator_to_file()
{
    //записываем в счётчик на +1
    QFile dataFile(outputDir + "/Info.dat");
    if(!dataFile.open(QIODevice::WriteOnly | QFile::Text))
    {
        emit sendString("Path not correct !!! ");
    }else{
        QTextStream stream(&dataFile);

        stream << QString::number( iterator);
        dataFile.flush();
        dataFile.close();

    }

}
//сохраняем график потенциала ф PNG
void File_Worker::save_potential_to_file()
{
    //Сохраняем первый график(потенциал)
    QString fileName = "potential_" +QString::number(iterator)  + ".png" ;
    QFile file(outputDir+"/"+fileName);

    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
    {

        emit sendString("Could not create Project File on disk");

    }else{
        emit sendString("Succes save to:  " + outputDir);
        emit save_graph_1(outputDir + "/" + fileName);
    }

}
//сохраняем график вероятностей в PNG
void File_Worker::save_propobility_to_file()
{
    //Сохраняем второй график(вероятности открытия/закрытия каналов)
    QString fileName = "propobility_" +QString::number(iterator)  + ".png" ;
    QFile file(outputDir+"/"+fileName);

    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
    {
        emit sendString("Could not create Project File on disk");


    }else{

        emit sendString("Succes save to:  " + outputDir);
        emit save_graph_2(outputDir + "/" + fileName);
    }
}
//сохраняем сами значения в TXT
void File_Worker::save_data_to_text(QVector<QVector <double> > &result)
{

    File_Worker::load_iterator_from_file();
    QString fileName = "text_data_" +QString::number(iterator)  + ".txt" ;
    QFile file(outputDir+"/"+fileName);

    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
    {
        emit sendString("Could not create Project File on disk");

    }else{

        emit sendString("Succes save to:  " + outputDir);

        QVector<double>::iterator it_vi = result[0].begin();
        QVector<double>::iterator it_n = result[1].begin();
        QVector<double>::iterator it_m = result[2].begin();
        QVector<double>::iterator it_h = result[3].begin();

        QTextStream stream(&file);

        for(it_vi, it_n, it_m, it_h;
            it_vi != result[0].end(),it_n != result[1].end(),
            it_m != result[2].end(),it_h != result[3].end();
            ++it_vi, ++it_n,++it_m,++it_h)
        {
            stream << *it_vi << "\t"
                   << *it_n << "\t"
                   << *it_m << "\t"
                   << *it_h << "\n";
        }

        file.flush();
        file.close();

    }

}
//сохраняем параметры по которым решалось ODE в TXT
void File_Worker::save_parameters_to_text(QVector<double> vec)
{
    QString fileName = "text_param_" +QString::number(iterator)  + ".txt" ;
    QFile file(outputDir+"/"+fileName);

    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
    {
        emit sendString("Could not create Project File on disk");

    }else{

        emit sendString("Succes save to:  " + outputDir);

        QTextStream stream(&file);

        QVector<double>::iterator it_vec = vec.begin();
        for(it_vec; it_vec != vec.end(); ++it_vec)
        {
            stream << *it_vec << "\n";

        }

        file.flush();
        file.close();


    }
}
//сохраняем значения токов в TXT
void File_Worker::save_current_to_file(QVector<QVector <double> > &result)
{

    File_Worker::load_iterator_from_file();
    QString fileName = "text_current_data_" +QString::number(iterator)  + ".txt" ;
    QFile file(outputDir+"/"+fileName);

    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
    {
        emit sendString("Could not create Project File on disk");

    }else{

        emit sendString("Succes save to:  " + outputDir);

        QVector<double>::iterator it_cur_K = result[0].begin();
        QVector<double>::iterator it_cur_Na = result[1].begin();
        QVector<double>::iterator it_cur_L = result[2].begin();

        QTextStream stream(&file);

        for(it_cur_K, it_cur_Na, it_cur_L;
            it_cur_K != result[0].end(),it_cur_Na != result[1].end(),
            it_cur_L != result[2].end();
            ++it_cur_K, ++it_cur_Na,++it_cur_L)
        {
            stream << *it_cur_K << "\t"
                   << *it_cur_Na << "\t"
                   << *it_cur_L << "\n";
        }

        file.flush();
        file.close();

    }

}
