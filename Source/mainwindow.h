#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "renderarea.h"
#include "define.h"
#include "QTimer"
#include "parser.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    sparameter_data adc_data;

private slots:
    void update_osil();
    void onPlotClick(int x, int y);
    void onMouseMove(int x, int y);

    void onNewFile(QString filename);

private:
    RenderArea *renderArea;
    int voltage;
    QLabel* status;
    int x;
    parser *device;
    QTimer *update_timer;


    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
};

#endif // MAINWINDOW_H
