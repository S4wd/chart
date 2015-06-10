#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QDir>
#include <QProgressDialog>
#include <QProgressBar>
#include <QtNetwork/QFtp>

#include "qcustomplot.h"
#include "tgchart.h"


#define DATA_DIR            "C:\\Users\\vxc\\Workspace S4wd\\chart\\s4wddata\\"
#define DATA_FILE           "C:\\Users\\vxc\\Workspace S4wd\\chart\\s4wddata\\%1nrd"

#define CSV_FILE            "C:\\Users\\vxc\\Workspace S4wd\\chart\\s4wddata\\%1s4wd"

struct Dplot
{
    QString alias;
    QVector<double> datapoints;
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title);
  void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
  void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
  void selectionChanged();
  void mousePress();
  void mouseWheel();
  void addRandomGraph();
  void removeSelectedGraph();
  void removeAllGraphs();
  void resetZoom();
  void contextMenuRequest(QPoint pos);
  void moveLegend();
  void graphClicked(QCPAbstractPlottable *plottable);

  void DownloadDataHandler();

  void ftpStateChanged(int state);
  void ftpFileGetProgress(qint64 done, qint64 total);
  void ftpFinished(int id, bool error);
  void progressdialogCancelled();

private:
  Ui::MainWindow *ui;


  /*Dplot latitude;
  Dplot longitude;
  Dplot bearing;
  Dplot altitiude;
  Dplot volts;
  Dplot amps;*/
  Dplot temperature;
  QVector<QString> datetime;

  QFtp ftp;
  QProgressDialog progressdialog;
  QFile * localfile;

  qint16 countDataFiles();
  void loadData();

};



#endif // MAINWINDOW_H

