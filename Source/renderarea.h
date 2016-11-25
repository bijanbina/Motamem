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
    int getYPoint(float y);
    int getPhasePoint(float y);

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
    bool isDouble;
    bool isPhase;
    double step_x;

signals:
    void click_on_point(int x,int y);
    void move_pointer(int x,float y);
    void drag_file(QString filename);
    void drop_multifile(QList<QUrl> urlList);
};
#endif // RENDERAREA_H
