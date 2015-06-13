#include <QRubberBand>
#include "tgchart.h"

TgChart::TgChart(QWidget * parent) :
    QCustomPlot(parent)
    , mZoomMode(true)
    , mRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{


    connect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(dataPointHint(QMouseEvent*)));
    tracerGraph = NULL;
}

TgChart::~TgChart()
{
    delete mRubberBand;
    delete tracerGraph;
}

void TgChart::setTracerGraph( QCPGraph * graph)
{
    tracerGraph = graph;
}

void TgChart::mousePressEvent(QMouseEvent * event)
{
    if (mZoomMode)
    {
        if ( event->button() == Qt::MiddleButton)
        {
            mOrigin = QPoint(event->pos().x(),axisRect()->topLeft().y());
            mRubberBand->setGeometry(QRect(mOrigin,
                                           QSize()));
            mRubberBand->show();
        }
    }
    QCustomPlot::mousePressEvent(event);
}

void TgChart::mouseMoveEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        mRubberBand->setGeometry(QRect(mOrigin, QPoint(event->pos().x(),axisRect()->bottomRight().y())).normalized());
    }
    QCustomPlot::mouseMoveEvent(event);
}

void TgChart::mouseReleaseEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        QRect zoomRect = mRubberBand->geometry();

        xAxis->setRange(xAxis->pixelToCoord( zoomRect.topLeft().x()), xAxis->pixelToCoord( zoomRect.bottomRight().x()));

        mRubberBand->hide();
        replot();
    }

    QCustomPlot::mouseReleaseEvent(event);
}

void TgChart::dataPointHint(QMouseEvent *event)
{
    if (tracerGraph == NULL)
        return;

    int x = xAxis->pixelToCoord(event->pos().x());
    QDateTime dt;
    dt.setTime_t(x);
    QString dateTime = dt.toString("ddd dd MM\nHH:mm:ss");
    double y = tracerGraph->data()->value(x).value;
    setToolTip(QString("%1 , %2").arg(dateTime).arg(y));
}


