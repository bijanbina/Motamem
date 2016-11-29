#include "mainwindow.h"

#include "renderarea.h"
#include "parser.h"
#include <string>
#include <math.h>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QUrl>
#include <QMessageBox>

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
    createMenu();

    x = 0;
    update_timer = new QTimer;
    connect(update_timer,SIGNAL(timeout()),this,SLOT(update_osil()));
    connect(renderArea,SIGNAL(click_on_point(int,int)),this,SLOT(onPlotClick(int,int)));
    connect(renderArea,SIGNAL(move_pointer(int,float)),this,SLOT(onMouseMove(int,float)));
    connect(renderArea,SIGNAL(drop_multifile(QList<QUrl>)),this,SLOT(dropMultiFile(QList<QUrl>)));

    connect(renderArea,SIGNAL(drag_file(QString)),this,SLOT(onNewFile(QString)));
    connect(A_About, SIGNAL(triggered(bool)),this,SLOT(about_clicked()));
    connect(renderArea, SIGNAL(phase_toggle()),this,SLOT(plot_phase_toggle()));
    connect(renderArea, SIGNAL(double_toggle()),this,SLOT(plot_double_toggle()));
    connect(A_plot_phase, SIGNAL(triggered(bool)),this,SLOT(Aplot_phase_toggle()));
    connect(A_plot_double, SIGNAL(triggered(bool)),this,SLOT(Aplot_double_toggle()));
}

void MainWindow::createMenu()
{
    menu = new QMenuBar;
    setMenuBar(menu);
    //File Menu
    QMenu *FileMenu = menu->addMenu(trUtf8("File"));
    A_Open = FileMenu->addAction(trUtf8("Open"));
    A_Save = FileMenu->addAction(trUtf8("Save"));
    A_Print  = FileMenu->addAction(trUtf8("Print"));
    A_Save->setEnabled(false);
    FileMenu->addSeparator();
    FileMenu->addSeparator();
    //Mode Menu in File
    QMenu *ModeMenu = FileMenu->addMenu(trUtf8("Generating Mode"));
    A_AutoMode     = ModeMenu->addAction(trUtf8("Auto"));
    A_ManualMode   = ModeMenu->addAction(trUtf8("Nanual"));
    //set Checkable Modes
    A_AutoMode->setCheckable(true);
    A_ManualMode->setCheckable  (true);

    FileMenu->addSeparator();
    A_Quit = FileMenu->addAction(trUtf8("Quit"));
    //Plot Menu
    //Plot Menu
    QMenu *plotMenu = menu->addMenu(trUtf8("Plot"));
    A_plot_phase     = plotMenu->addAction(trUtf8("Plot Phase"));
    A_plot_double    = plotMenu->addAction(trUtf8("Double Plot"));

    A_plot_phase->setCheckable (true);
    A_plot_double->setCheckable(true);
    //Signal Menu
    QMenu *SigMenu  = menu->addMenu(trUtf8("Signal"));
    A_SetTime       = SigMenu->addAction(trUtf8("Signal Time"));
    A_SetDB         = SigMenu->addAction(trUtf8("Database Path"));
    A_SetRecord     = SigMenu->addAction(trUtf8("Record Number"));
    B_AskComplex    = SigMenu->addAction(trUtf8("Ask Complex Number"));

    //Help Menu
    QMenu *HelpMenu = menu->addMenu(trUtf8("Help"));
    A_Content       = HelpMenu->addAction(trUtf8("Content"));
    A_Support       = HelpMenu->addAction(trUtf8("Support"));
    A_Update        = HelpMenu->addAction(trUtf8("Update"));
    A_About         = HelpMenu->addAction(trUtf8("About"));
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

void MainWindow::onMouseMove(int x, float y)
{
    if (A_plot_phase->isChecked())
    {
        status->setText("  " + QString::number(x) + "MHz : -" + QString::number(y, 'g', 4) + " °");
    }
    else
    {
        status->setText("  " + QString::number(x) + "MHz : -" + QString::number(y, 'g', 4) + " dB");
    }
}

void MainWindow::onNewFile(QString filename)
{
    device->openFile(filename);
    update_timer->start(30);
}

void MainWindow::dropMultiFile(QList<QUrl> urlList)
{
    for (int i = 0 ; i < urlList.size() ; i++)
    {
        device->openFile(urlList.at(i).toLocalFile());
    }
}

void MainWindow::about_clicked()
{
    QMessageBox about_box(this);
    about_box.setText(\
    "Bijan Binaee\nMicrowave Laboratory\nMotamem Project\nCenter of Excellence on Applied Electromagnetic Systems\nCopyright © 2016 University of Tehran.\nAll rights reserved.");
    about_box.setIconPixmap(QPixmap(":/Resources/ut_logo.png"));
    about_box.setWindowTitle("About");
    about_box.exec();
}

void MainWindow::plot_phase_toggle()
{
    device->isPhase = renderArea->isPhase;
    A_plot_phase->setChecked(device->isPhase);
}

void MainWindow::plot_double_toggle()
{
    device->isDouble = renderArea->isDouble;
    A_plot_double->setChecked(device->isDouble);
}

void MainWindow::Aplot_phase_toggle()
{
    renderArea->isPhase = A_plot_phase->isChecked();
    device->isPhase = renderArea->isPhase;
}

void MainWindow::Aplot_double_toggle()
{
    renderArea->isDouble = A_plot_double->isChecked();
    device->isDouble = renderArea->isDouble;
}
