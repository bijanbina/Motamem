#include "renderarea.h"
#include "parser.h"
#include <QtWidgets>
#include <string>
#include <math.h>
#include <QDebug>
#include <qcustomplot.h>

parser::parser(sparameter_data *data)
{
    //serial = new QSerialPort(this);
    //x = 0;
    //turn = find_backR;
    plot_data = data;
    isDouble = false;
    isPhase = false;
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
    plot_data->f_start = fields.at(1).toLongLong()/1000/1000;
    plot_data->f_end = fields.at(2).toLongLong()/1000/1000;
    plot_data->point_count = fields.at(3).toLong();
    plot_data->freq = QVector<int> (plot_data->point_count);
    plot_data->step = ( plot_data->f_end - plot_data->f_start ) / (plot_data->point_count * 1.0);
    for (int i = 0 ; i < plot_data->point_count ; i++)
    {
        plot_data->freq[i] = plot_data->f_start + plot_data->step * i;
    }
    plot_data->S11 = QVector<float> (plot_data->point_count);
    plot_data->S12 = QVector<float> (plot_data->point_count);
    plot_data->S21 = QVector<float> (plot_data->point_count);
    plot_data->S22 = QVector<float> (plot_data->point_count);
    plot_data->S11_phase = QVector<float> (plot_data->point_count);
    plot_data->S12_phase = QVector<float> (plot_data->point_count);
    plot_data->S21_phase = QVector<float> (plot_data->point_count);
    plot_data->S22_phase = QVector<float> (plot_data->point_count);
    while(!testStream.atEnd())
    {
        while(line != "BEGIN" )
        {
            line = testStream.readLine();
            //qDebug() << line;
        }
        int i = 0;
        line = testStream.readLine();
        while(!line.contains("END") )
        {
            //qDebug() << line;
            QStringList fields = line.split(",");
            float s_buffer = sqrt((pow(fields.at(0).toFloat(),2) + pow(fields.at(1).toFloat(),2)));
            float p_buffer = atan(fields.at(1).toFloat()/fields.at(0).toFloat());
            switch (read_param)
            {
                case 0:
                    plot_data->S11[i] = s_buffer;
                    plot_data->S11_phase[i] = p_buffer;
                    break;
                case 1:
                    plot_data->S12[i] = s_buffer;
                    plot_data->S12_phase[i] = p_buffer;
                    break;
                case 2:
                    plot_data->S21[i] = s_buffer;
                    plot_data->S21_phase[i] = p_buffer;
                    break;
                case 3:
                    plot_data->S22[i] = s_buffer;
                    plot_data->S22_phase[i] = p_buffer;
                    break;
            }


            i++;
            line = testStream.readLine();
        }
        read_param++;
    }
    createPlotFiles(filename);

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

void parser::createPlotFiles(QString filename)
{
    PlotFileS(filename,S11_PLOT);
    PlotFileS(filename,S12_PLOT);
    if (isDouble == 0)
    {
        PlotFileS(filename,S21_PLOT);
        PlotFileS(filename,S22_PLOT);
    }
}

//s parameter plot
void parser::PlotFileS(QString filename,plotID plot_id)
{
    QCustomPlot *plot_wid = new QCustomPlot;
    QFileInfo *file_info = new QFileInfo(filename);
    QVector <double> s_10log = QVector<double> (plot_data->point_count);
    QVector <double> f = QVector<double> (plot_data->point_count);
    QString plot_title = file_info->baseName();
    QString base_path = file_info->absoluteDir().absolutePath() + "/" + file_info->baseName();
    QString plot_s_name;
    QVector<float> *s_param;

    switch(plot_id)
    {
        case S11_PLOT:
            plot_s_name = "S11";
            s_param = &(plot_data->S11);
            break;
        case S12_PLOT:
            plot_s_name = "S12";
            s_param = &(plot_data->S12);
            break;
        case S21_PLOT:
            plot_s_name = "S21";
            s_param = &(plot_data->S21);
            break;
        case S22_PLOT:
            plot_s_name = "S22";
            s_param = &(plot_data->S22);
            break;
    }

    double s_min = 0;
    for (int i = 0 ; i < plot_data->point_count ; i++)
    {
        s_10log[i] = 10 * log10((*s_param)[i]);
        f[i] = plot_data->freq[i];
        if (s_min > s_10log[i])
        {
            s_min = s_10log[i];
        }
    }

    plot_wid->addGraph();
    plot_wid->graph(0)->setData(f,s_10log);

    if (isDouble)
    {    switch(plot_id)
        {
            case S11_PLOT:
                s_param = &(plot_data->S22);
                break;
            case S12_PLOT:
                s_param = &(plot_data->S21);
                break;
            case S21_PLOT:
                s_param = &(plot_data->S12);
                break;
            case S22_PLOT:
                s_param = &(plot_data->S11);
                break;
        }

        for (int i = 0 ; i < plot_data->point_count ; i++)
        {
            s_10log[i] = 10 * log10((*s_param)[i]);
        }
        plot_wid->addGraph();
        plot_wid->graph(1)->setData(f,s_10log);
        plot_wid->graph(1)->setPen(Qt::DashLine);
        plot_s_name += " D";
    }

    plot_wid->xAxis->setLabel("Frequency (MHz)");
    plot_wid->yAxis->setLabel("Power (dB)");
    plot_wid->plotLayout()->insertRow(0);
    plot_wid->plotLayout()->addElement(0,0, new QCPTextElement(plot_wid,
                                                   plot_title + " " + plot_s_name,
                                                   QFont("sans", 12, QFont::Bold)));

    plot_wid->xAxis->setRange(plot_data->f_start, plot_data->f_end);
    plot_wid->yAxis->setRange(floor(s_min*1.4142),5);
    plot_wid->replot();
    //qDebug() << file_info->absoluteDir().absolutePath();
    plot_wid->savePng(base_path + "_" + plot_s_name + ".png");
}
