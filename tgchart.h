#ifndef TGCHART_H
#define TGCHART_H


#include <QPoint>
#include "qcustomplot.h"

class QRubberBand;
class QMouseEvent;
class QWidget;


class TgChart : public QCustomPlot
{
    Q_OBJECT

public:
    TgChart(QWidget * parent = 0);
    virtual ~TgChart();

    void setTracerGraph( QCPGraph * graph);
    void setTracerVisible(bool visible);

private slots:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    void dataPointHint(QMouseEvent *event);

private:
    bool mZoomMode;
    QRubberBand * mRubberBand;
    QPoint mOrigin;
    QCPGraph * tracerGraph;
};

#endif // TGCHART_H
