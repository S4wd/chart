#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QDir>
#include <QProgressDialog>
#include <QProgressBar>
#include <QtNetwork/QFtp>
#include <QModelIndex>
#include <QMenu>

#include "qcustomplot.h"
#include "tgchart.h"


#define A20_S4WD_HOST_IP    "192.168.40.1"
#define A20_S4WD_FTP_LOGIN  "root"
#define A20_S4WD_FTP_PASS   "olimex"

#define A20_S4WD_LOGGING_DIR "/home/s4wd/logging"
#define A20_S4WD_LOGGING_FILE "/home/s4wd/logging/%1"

#define LOCAL_LOGGING_FILE    "s4wddata\\%1"
#define LOCAL_LOGGING_DIR     "s4wddata"



enum DataProfileType {dpCharge, dpDischarge};

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
  //void addRandomGraph();
  void removeSelectedGraph();
  void removeAllGraphs();
  void resetZoom();
  void contextMenuRequest(QPoint pos);
  void moveLegend();
  void graphClicked(QCPAbstractPlottable *plottable);





  void ftpStateChanged(int state);
  void ftpFileGetProgress(qint64 done, qint64 total);
  void ftpFinished(int id, bool error);
  void ftpUrlFound(QUrlInfo url);

  void on_listWidget_doubleClicked(const QModelIndex &index);

  void loadSessionChooserPage();

private:
  Ui::MainWindow *ui;


  Dplot motorT;
  Dplot controllerT;
  Dplot volts;
  Dplot amps;
  Dplot speed;
  Dplot altitude;
  //Dplot temperature;
  QVector<QString> datetime;

  QStringList localFiles;
  QStringList newFiles;
  int filesUploaded;

  QFtp ftp;
  QProgressDialog progressdialog;
  QFile * localfile;

  void ftpListFiles();
  void localListFiles();

  void ftpGetFile();
  qint16 countDataFiles();
  void loadData(QString filepath, DataProfileType profile);
  void addPlot(Dplot &plot);


  void ftpSetup();
  void chartSetup();



};



#endif // MAINWINDOW_H

