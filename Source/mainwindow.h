#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "renderarea.h"
#include "define.h"
#include "QTimer"
#include "parser.h"
#include "QMenuBar"


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
    void onMouseMove(int x, float y);
    void dropMultiFile(QList<QUrl> urlList);


    void onNewFile(QString filename);
    void about_clicked();

private:
    void createMenu();

    QMenuBar *menu;
    QMenu *FileMenu;
    QAction *A_Open;
    QAction *A_Save;
    QAction *A_LoadInfo;
    QAction *A_Print;
    QMenu *ModeMenu;
    QAction *A_AutoMode;
    QAction *A_ManualMode;
    QAction *A_Quit;
    //Plot Menu
    QMenu *plotMenu;
    QAction *A_plot_phase;
    QAction *A_plot_double;
    //Signal Menu
    QMenu *SigMenu;
    QAction *A_SetTime;
    QAction *A_SetDB;
    QAction *A_SetRecord;
    QAction *B_AskComplex;
    //Help Menu
    QMenu *HelpMenu;
    QAction *A_Content;
    QAction *A_Support;
    QAction *A_Update;
    QAction *A_About;

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
