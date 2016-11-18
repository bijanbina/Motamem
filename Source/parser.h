#ifndef OSIL_H
#define OSIL_H

#include "define.h"
#include <QWidget>
#include <QtSerialPort/QSerialPort>

class parser : public QObject
{
    Q_OBJECT

public:
    parser(sparameter_data *data);
    ~parser();
    bool openFile(QString filename);
    void createPlotFile();

private:
    sparameter_data *plot_data;
};


#endif // OSIL_H


