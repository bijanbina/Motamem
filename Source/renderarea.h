#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include "define.h"
//! [0]
//!

class RenderArea : public QWidget
{
    Q_OBJECT

public:

    RenderArea(sparameter_data *data, QWidget *parent = 0);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void renderPoint(QPainter *painter);

    bool isDouble;
    bool isPhase;

private slots:
    void calc_fps();


protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;

private:
    int getY_Mag(float mag);
    int getY_Phase(float phase);
    float getPhase_Y(int y);
    float getMag_Y(int y);
    void updateChannel();

    sparameter_data *adc_data;
    int i,frameNumber,fps;
    QPixmap *boom;
    QPainter img_painter;
    QPixmap *background;
    QTimer *fps_timer;
    QString plot_filename;
    plotID plot_id;
    QPoint cursor;
    bool cursor_enable;
    bool cursor_lock;
    double step_x;
    QVector<float> *channel1;
    QVector<float> *channel2;
    QVector<float> *channel1_phase;
    QVector<float> *channel2_phase;

signals:
    void click_on_point(int x,int y);
    void move_pointer(int x,float y);
    void drag_file(QString filename);
    void drop_multifile(QList<QUrl> urlList);
    void phase_toggle();
    void double_toggle();
};
#endif // RENDERAREA_H
