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

    void setZoomMode(bool mode);

    void setTracerGraph( QCPGraph * graph);
    void setTracerVisible(bool visible);

private slots:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

    void dataPointHint(QMouseEvent *event);

private:
    bool mZoomMode;
    QRubberBand * mRubberBand;
    QCPItemTracer * tracer;
    QPoint mOrigin;
};

#endif // TGCHART_H
