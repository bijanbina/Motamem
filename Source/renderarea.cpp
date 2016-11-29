#include "renderarea.h"

#include <QPainter>
#include <QMimeData>
#include <qmath.h>
#include <QInputDialog>


RenderArea::RenderArea(sparameter_data *data, QWidget *parent)
    : QWidget(parent)
{
    adc_data = data;
    cursor_enable = false;
    cursor_lock = false;
    step_x = screen_size/screen_res;
    this->setMouseTracking(true);
    this->setAcceptDrops(true);
    plot_id = NO_PLOT;
    this->setFocusPolicy(Qt::StrongFocus);
    qDebug() << "H: " << this->acceptDrops();
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    //painter = QPainter(this);
    /*boom = new QPixmap (1200, 400);
    img_painter.begin(boom);
    boom->fill(Qt::transparent);*/
    background = new QPixmap(":Resources/background.png");
    fps_timer = new QTimer;
    fps_timer->start(1000);
    connect(fps_timer,SIGNAL(timeout()),this,SLOT(calc_fps()));
    fps = 30;
    isDouble = false;
    isPhase = false;
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(screen_size, 400);
}

QSize RenderArea::sizeHint() const
{
    return QSize(screen_size, 400);
}

void RenderArea::calc_fps()
{
    fps = frameNumber;
    frameNumber = 0;
}

void RenderArea::renderPoint(QPainter *painter)
{
    painter->setBrush(QColor("#50bdd2"));
    painter->setPen(QColor("#50bdd2"));
    float y1,y2;
    if (isPhase)
    {
        for (int x = 1; x < adc_data->point_count; x++)
        {
            y2 = (*channel1_phase)[x];
            y1 = (*channel1_phase)[x-1];
            //painter->drawEllipse(QPoint(10*x+5,y),5,5);
            painter->drawLine(step_x*x-step_x/2,getY_Phase(y1),step_x*x+step_x/2,getY_Phase(y2));
        }

        if (isDouble)
        {
            painter->setBrush(QColor("#d25079"));
            painter->setPen(QColor("#d25079"));
            for (int x = 1; x < adc_data->point_count; x++)
            {
                y2 = (*channel2_phase)[x];
                y1 = (*channel2_phase)[x-1];
                painter->drawLine(step_x*x-step_x/2,getY_Phase(y1),step_x*x+step_x/2,getY_Phase(y2));
            }
        }
    }
    else
    {
        for (int x = 1; x < adc_data->point_count; x++)
        {
            y2 = (*channel1)[x];
            y1 = (*channel1)[x-1];
            painter->drawLine(step_x*x-step_x/2,getY_Mag(y1),step_x*x+step_x/2,getY_Mag(y2));
        }

        if (isDouble)
        {
            painter->setBrush(QColor("#d25079"));
            painter->setPen(QColor("#d25079"));
            for (int x = 1; x < adc_data->point_count; x++)
            {
                y2 = (*channel2)[x];
                y1 = (*channel2)[x-1];
                //painter->drawEllipse(QPoint(10*x+5,y),5,5);
                if (true)
                {
                    painter->drawLine(step_x*x-step_x/2,getY_Mag(y1),step_x*x+step_x/2,getY_Mag(y2));
                }
                else
                {
                    painter->drawLine(step_x*x-step_x/2,y1,step_x*x+step_x/2,y2);
                }
            }
        }
    }
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter;
    QPixmap image(this->size());
    painter.begin(&image);
    painter.drawPixmap(0,0,this->size().width(),this->size().height(),*background);
    renderPoint(&painter);
    painter.end();

    QPainter painter_wid;
    painter_wid.begin(this);
    painter_wid.drawPixmap(0,0,image);
    painter_wid.setPen(QColor("#295a83"));
    painter_wid.drawText(10,15,QString::number(fps));

    if (plot_filename.length())
    {
        int pixelsWide = painter_wid.fontMetrics().width(plot_filename); //calculate rendered test width
        painter_wid.drawText(size().width()-pixelsWide-10,15,plot_filename);
    }
    switch(plot_id)
    {
        case S11_PLOT:
            painter_wid.drawText(10,size().height()-15,"S11");
            break;
        case S12_PLOT:
            painter_wid.drawText(10,size().height()-15,"S12");
            break;
        case S21_PLOT:
            painter_wid.drawText(10,size().height()-15,"S21");
            break;
        case S22_PLOT:
            painter_wid.drawText(10,size().height()-15,"S22");
            break;
    }
    if (isPhase)
    {
        painter_wid.drawText(30,15,"P");
        if (isDouble)
        {
            painter_wid.drawText(35,15,", D");
        }
    }
    else if (isDouble)
    {
        painter_wid.drawText(30,15,"D");
    }
    if (cursor_enable)
    {
        painter_wid.setPen(QColor("#50d2a5"));
        painter_wid.setBrush(QBrush(QColor("#50d2a5")));
        painter_wid.drawEllipse(cursor,4,4);
    }

    frameNumber++;
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && adc_data->point_count != 0) {

        if ( cursor_lock )
        {
            cursor_lock = false;
            return;
        }

        emit click_on_point(event->x(),event->y());
        cursor_enable = !cursor_enable;
        cursor.setX(event->x());
        if ( qRound(event->x()/step_x) < adc_data->point_count )
        {
            cursor.setY(getY_Mag((*channel1)[qRound(event->x()/step_x)]));
        }
        else
        {
            cursor.setY(getY_Mag((*channel1)[adc_data->point_count-1]));
        }
    }
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    if ( !cursor_lock )
    {
        cursor.setX(event->x());
        if ( qRound(event->x()/step_x) < adc_data->point_count )
        {
            if (isPhase)
            {
                cursor.setY(getY_Phase((*channel1_phase)[qRound(event->x()/step_x)]));
                if (cursor_enable)
                {
                    emit move_pointer(adc_data->f_start + event->x() /step_x * adc_data->step,getPhase_Y(cursor.y()));
                }
                else
                {
                    emit move_pointer(event->x(),event->y());
                }
            }
            else
            {
                cursor.setY(getY_Mag((*channel1)[qRound(event->x()/step_x)]));
                if (cursor_enable)
                {
                    emit move_pointer(adc_data->f_start + event->x() /step_x * adc_data->step,getMag_Y(cursor.y()));
                }
                else
                {
                    emit move_pointer(event->x(),event->y());
                }
            }
        }
    }

}

void RenderArea::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

int RenderArea::getY_Mag(float mag)
{
    if (true)
    {
        return  round(-log10(mag)*10*12 + TOPBAR_OFFSET);
    }
    else
    {
        return round(height()-mag*400 + TOPBAR_OFFSET);
    }
}

int RenderArea::getY_Phase(float phase)
{
   return  round(phase/3.14159265*180.0 * 0.66 + 200);
}

float RenderArea::getPhase_Y(int y)
{
   return  (y - 200.0) * 1.5;
}

float RenderArea::getMag_Y(int y)
{
   return  (y - TOPBAR_OFFSET)/ 12.0;
}


void RenderArea::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    this->window()->activateWindow();
    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
      QStringList pathList;
      QList<QUrl> urlList = mimeData->urls();

      // extract the local paths of the files
      /*for (int i = 0; i < urlList.size() && i < 32; +i)
      {
        pathList.append(urlList.at(i).toLocalFile());
      }

      // call a function to open the files
      openFiles(pathList);*/
      if (urlList.size() > 1)
      {
          emit drop_multifile(urlList);
      }
      else
      {
          plot_filename = urlList.at(0).toLocalFile();
          emit drag_file(plot_filename);
          plot_id = S11_PLOT;
          updateChannel();
      }
    }
}

//update plot channel vector to match plot id
void RenderArea::updateChannel()
{
    switch(plot_id)
    {
        case S11_PLOT:
            channel1 = &(adc_data->S11);
            channel2 = &(adc_data->S22);
            channel1_phase = &(adc_data->S11_phase);
            channel2_phase = &(adc_data->S22_phase);
            break;
        case S12_PLOT:
            channel1 = &(adc_data->S12);
            channel2 = &(adc_data->S21);
            channel1_phase = &(adc_data->S12_phase);
            channel2_phase = &(adc_data->S21_phase);
            break;
        case S21_PLOT:
            channel1 = &(adc_data->S21);
            channel2 = &(adc_data->S12);
            channel1_phase = &(adc_data->S21_phase);
            channel2_phase = &(adc_data->S12_phase);
            break;
        case S22_PLOT:
            channel1 = &(adc_data->S22);
            channel2 = &(adc_data->S11);
            channel1_phase = &(adc_data->S22_phase);
            channel2_phase = &(adc_data->S11_phase);
            break;
    }
}

void RenderArea::keyPressEvent(QKeyEvent * event)
{
    if( event->key() == Qt::Key_1 )
    {
        plot_id = S11_PLOT;
        updateChannel();
    }
    else if( event->key() == Qt::Key_2 )
    {
        plot_id = S12_PLOT;
        updateChannel();
    }
    else if( event->key() == Qt::Key_3 )
    {
        plot_id = S21_PLOT;
        updateChannel();
    }
    else if( event->key() == Qt::Key_4 )
    {
        plot_id = S22_PLOT;
        updateChannel();
    }
    else if( event->key() == Qt::Key_L )
    {
        bool ok;
        double freq = QInputDialog::getDouble(this->window(), "Frequency Measurment",
                                             tr("Enter Frequency:"), adc_data->f_start, adc_data->f_start, adc_data->f_end,0, &ok);
        if (ok)
        {
            cursor_enable = true;
            cursor_lock = true;
            cursor.setX((freq - adc_data->f_start) / adc_data->step * step_x);
        }
    }
    else if( event->key() == Qt::Key_D )
    {
        isDouble = !isDouble;
        emit double_toggle();
    }
    else if( event->key() == Qt::Key_P )
    {
        isPhase = !isPhase;
        emit phase_toggle();
    }
    int a = qRound(cursor.x()/step_x);
    if (cursor_enable)
    {   //update for new parameter
        if (isPhase)
        {
            cursor.setY(getY_Phase((*channel1_phase)[qRound(cursor.x()/step_x)]));
            emit move_pointer(adc_data->f_start + cursor.x() /step_x * adc_data->step,getPhase_Y(cursor.y()));
        }
        else
        {
            cursor.setY(getY_Mag((*channel1)[qRound(cursor.x()/step_x)]));

            emit move_pointer(adc_data->f_start + cursor.x() /step_x * adc_data->step,getMag_Y(cursor.y()));
        }
    }

}
