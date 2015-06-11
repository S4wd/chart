#include <QRubberBand>
#include "tgchart.h"

TgChart::TgChart(QWidget * parent) :
    QCustomPlot(parent)
    , mZoomMode(true)
    , mRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{


    connect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(dataPointHint(QMouseEvent*)));
    tracer = new QCPItemTracer(this);
    //addItem(tracer);
    tracer->setStyle(QCPItemTracer::tsCircle);
    tracer->setVisible(true);
    tracer->setPen(QPen(QColor(Qt::white)));
    tracer->setInterpolating(false);
    tracer->setGraphKey(1);


}

TgChart::~TgChart()
{
    delete mRubberBand;
}

void TgChart::setZoomMode(bool mode)
{
    mZoomMode = mode;
}

void TgChart::setTracerGraph( QCPGraph * graph)
{
    tracer->setGraph(graph);
}

void TgChart::setTracerVisible(bool visible)
{
    tracer->setVisible(visible);
}

void TgChart::mousePressEvent(QMouseEvent * event)
{
    if (mZoomMode)
    {
        if ( event->button() == Qt::MiddleButton)
        {
            mOrigin = event->pos();
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
        mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
    }
    QCustomPlot::mouseMoveEvent(event);
}

void TgChart::mouseReleaseEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        const QRect & zoomRect = mRubberBand->geometry();
        int xp1, yp1, xp2, yp2;
        zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
        double x1 = xAxis->pixelToCoord(xp1);
        double x2 = xAxis->pixelToCoord(xp2);
        double y1 = yAxis->pixelToCoord(yp1);
        double y2 = yAxis->pixelToCoord(yp2);

        xAxis->setRange(x1, x2);
        yAxis->setRange(y1, y2);

        mRubberBand->hide();
        replot();
    }

    QCustomPlot::mouseReleaseEvent(event);
}

void TgChart::dataPointHint(QMouseEvent *event)
{
    int x = this->xAxis->pixelToCoord(event->pos().x());
    int y = this->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(x).arg(y));
}


