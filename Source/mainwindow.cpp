#include "mainwindow.h"

#include "renderarea.h"
#include "parser.h"
#include <string>
#include <math.h>
#include <QtSerialPort/QSerialPort>
#include <QDebug>

#define port_name       "ttyUSB0"
#define baud_rate       QSerialPort::Baud9600
#define data_bits       QSerialPort::Data8
#define parity          QSerialPort::NoParity
#define stop_bits       QSerialPort::OneStop
#define flow_control    QSerialPort::NoFlowControl

#define find_backR      0
#define get_first_char  1
#define get_sec_char    2


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    device = new parser(&(adc_data));
    renderArea = new RenderArea(&(adc_data));
    status = new QLabel("  Disconnected");
    status->setStyleSheet("color: #fff");
    mainWidget = new QWidget;
    mainWidget->setStyleSheet("background-color: #2f343f");
    mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(renderArea);
    mainLayout->addWidget(status);
    mainLayout->addSpacing(5);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setWindowTitle("Motamem");
    setWindowFlags(Qt::WindowStaysOnTopHint);
    adc_data.point_count = 0;

    x = 0;
    update_timer = new QTimer;
    connect(update_timer,SIGNAL(timeout()),this,SLOT(update_osil()));
    connect(renderArea,SIGNAL(click_on_point(int,int)),this,SLOT(onPlotClick(int,int)));
    connect(renderArea,SIGNAL(move_pointer(int,int)),this,SLOT(onMouseMove(int,int)));

    connect(renderArea,SIGNAL(drag_file(QString)),this,SLOT(onNewFile(QString)));

    update_timer->start(30);



}

void MainWindow::update_osil()
{
    renderArea->update();
}

MainWindow::~MainWindow()
{
    //device->closeSerialPort();
}

void MainWindow::onPlotClick(int x, int y)
{
    //status->setText("  " + QString::number(x) + ", " + QString::number(y));
}

void MainWindow::onMouseMove(int x, int y)
{
    status->setText("  " + QString::number(x) + ", " + QString::number(y));
}

void MainWindow::onNewFile(QString filename)
{
    device->openFile(filename);
}
