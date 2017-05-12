#ifndef HELP_BROWSER_H
#define HELP_BROWSER_H

#include <QWidget>
#include <QtWidgets>

class Help_Browser : public QWidget
{
    Q_OBJECT
public:
    explicit Help_Browser(const QString& ,
                          const QString& ,
                          QWidget *parent = 0);

};

#endif // HELP_BROWSER_H
