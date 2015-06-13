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

#define LOCAL_LOGGING_FILE    "C:\\Users\\Vaughn\\Workspace S4wd\\chart\\debug\\s4wddata\\%1"
#define LOCAL_LOGGING_DIR     "C:\\Users\\Vaughn\\Workspace S4wd\\chart\\debug\\s4wddata"


enum DriveAxisType {daTemperature=0, daAmps=1, daVolts=2, daSpeed=3, daAltitude=4};
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

  void selectionChanged();
  void mousePress();
  void mouseWheel();

  void removeAllGraphs();
  void resetZoom();
  void contextMenuRequest(QPoint pos);
  //void graphClicked(QCPAbstractPlottable *plottable);





  void ftpStateChanged(int state);
  void ftpFileGetProgress(qint64 done, qint64 total);
  void ftpFinished(int id, bool error);
  void ftpUrlFound(QUrlInfo url);

  void on_listWidget_doubleClicked(const QModelIndex &index);

  void loadSessionChooserPage();

  void on_syncOkButton_clicked();

  void on_syncNoButton_clicked();

private:
  Ui::MainWindow *ui;

  QVector<double> time;
  Dplot motorT;
  Dplot controllerT;
  Dplot volts;
  Dplot amps;
  Dplot speed;
  Dplot altitude;
  //Dplot temperature;
  QVector<QString> datetime;
  const QColor drivePlotColors[6] = {QColor(0,255,0), QColor(245,245,0), QColor(255,0,0),QColor(0,255,255) , QColor(255,0,127), QColor(255,85,0)};
  QVector<QCPAxis *> driveAxis;


  QStringList localFiles;
  QStringList newFiles;
  int filesUploaded;

  QFtp ftp;
  QFile * localfile;

  void ftpListFiles();
  void localListFiles();

  void ftpGetFile();
  qint16 countDataFiles();
  void loadData(QString filepath, DataProfileType profile);
  void addPlot(Dplot &plot, QColor plotColor, DriveAxisType axisIndex);
  void setupAxis();

  void ftpSetup();
  void chartSetup();



};



#endif // MAINWINDOW_H

