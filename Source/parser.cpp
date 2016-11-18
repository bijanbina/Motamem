#include "renderarea.h"
#include "parser.h"
#include <QtWidgets>
#include <string>
#include <math.h>
#include <QtSerialPort/QSerialPort>
#include <QDebug>

#define port_name       "ttyUSB0"
#define baud_rate       QSerialPort::Baud115200
#define data_bits       QSerialPort::Data8
#define parity          QSerialPort::NoParity
#define stop_bits       QSerialPort::OneStop
#define flow_control    QSerialPort::NoFlowControl

#define find_backR      0
#define get_first_char  1
#define get_sec_char    2

parser::parser(sparameter_data *data)
{
    //serial = new QSerialPort(this);
    //x = 0;
    //turn = find_backR;
    plot_data = data;
    //connect(serial,SIGNAL(readyRead()),this,SLOT(readData()));
}

bool parser::openFile(QString filename)
{

    unsigned int voltage;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString());
    }
    QTextStream testStream(&file);

    int read_param = 0;
    QString line = testStream.readLine();
    while( !line.contains("SEG ") && !testStream.atEnd())
    {
        line = testStream.readLine();
    }
    QStringList fields = line.split(" ");
    plot_data->f_start = fields.at(1).toLong()/1000/1000;
    plot_data->f_end = fields.at(2).toLong()/1000/1000;
    plot_data->point_count = fields.at(3).toLong();
    plot_data->freq = QVector<int> (plot_data->point_count);
    int step = ( plot_data->f_end - plot_data->f_start ) / plot_data->point_count;
    for (int i = 0 ; i < plot_data->point_count ; i++)
    {
        plot_data->freq[i] = plot_data->f_start + step * i;
    }
    plot_data->S11 = QVector<int> (plot_data->point_count);
    plot_data->S12 = QVector<int> (plot_data->point_count);
    plot_data->S21 = QVector<int> (plot_data->point_count);
    plot_data->S22 = QVector<int> (plot_data->point_count);
    while(!testStream.atEnd())
    {
        while(line != "BEGIN" )
        {
            line = testStream.readLine();
            //qDebug() << line;
        }
        qDebug() << "DETECTED";
        int i = 0;
        line = testStream.readLine();
        while(!line.contains("END") )
        {
            //qDebug() << line;
            QStringList fields = line.split(",");
            switch (read_param)
            {
                case 0:
                    plot_data->S11[i] = (pow(fields.at(0).toFloat(),2) + pow(fields.at(1).toFloat(),2))* 2000 + TOPBAR_OFFSET;
                    break;
                case 1:
                    plot_data->S12[i] = (pow(fields.at(0).toFloat(),2) + pow(fields.at(1).toFloat(),2))* 300 + TOPBAR_OFFSET;
                    break;
                case 2:
                    plot_data->S21[i] = (pow(fields.at(0).toFloat(),2) + pow(fields.at(1).toFloat(),2))* 300 + TOPBAR_OFFSET;
                    break;
                case 3:
                    plot_data->S22[i] = (pow(fields.at(0).toFloat(),2) + pow(fields.at(1).toFloat(),2))* 2000 + TOPBAR_OFFSET;
                    break;
            }


            i++;
            line = testStream.readLine();
        }
        read_param++;
    }
    /*while (true)
    {
        data = serial->read(1);
        switch (turn)
        {
            case find_backR:
                voltage = 0;
                if(data.at(0) == '\r')
                    turn = get_first_char;
                break;
            case get_first_char:
                voltage = (unsigned char)data.at(0) & 0xff;
                turn = get_sec_char;
                break;
            case get_sec_char:
                if (voltage > 256)
                {
                    turn = find_backR;
                    break;
                }
                voltage += (((unsigned char)data.at(0)) & 0xf)*256;
                turn = find_backR;
                //qDebug()<< "x: " << x << " voltage: " <<voltage;
                adc_data->data[x] = 340.0-(voltage/4096.0 * 300.0) + 10;
                //if (adc_data->data[x] < 5)
                //{
                //    qDebug() << "voltage :" << voltage << "x: " << x << " adc_data->data[x]:" << adc_data->data[x];
                //}
                if(x>screen_res)
                {
                    x = 0;
                }
                else
                {
                    x++;
                }
                adc_data->buffer++;
                if(adc_data->buffer > screen_res)
                {
                    adc_data->buffer = screen_res;
                }
                break;
        }
    }
    */
}

parser::~parser()
{
    ;
}

void parser::createPlotFile()
{

}
