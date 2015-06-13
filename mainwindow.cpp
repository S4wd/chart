#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  srand(QDateTime::currentDateTime().toTime_t());
  ui->setupUi(this);

  ui->progressBar->setVisible(false);
  ui->stackedWidget->setCurrentIndex(0);


  ftpSetup();
  chartSetup();

}

MainWindow::~MainWindow()
{
    foreach (QCPAxis *axis, driveAxis) {
        qDebug() << axis->label();
        delete axis;
    }
    driveAxis.clear();
    delete localfile;
    delete ui;
}




void MainWindow::selectionChanged()
{
  bool selected  = false;
  // synchronize selection of graphs with selection of corresponding legend items and axis:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);

    if (item->selected() || graph->selected() )
    {
      item->setSelected(true);
      graph->setSelected(true);
      graph->valueAxis()->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels|QCPAxis::spAxisLabel);
      ui->customPlot->setTracerGraph(graph);
      selected = true;
    }
    else
        graph->valueAxis()->setSelectedParts(QCPAxis::spNone);
  }

  if (!selected)
      ui->customPlot->setTracerGraph(NULL);

}

void MainWindow::mousePress()
{
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
}

void MainWindow::mouseWheel()
{
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
}

void MainWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
}

void MainWindow::resetZoom()
{
   ui->customPlot->xAxis->setRange(time[0], time[time.count()-1]);
   ui->customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setStyleSheet("QMenu { font-size:11px; width: 150px; color:white; left: 20px; background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #cccccc, stop: 1 #333333);}");
  menu->setAttribute(Qt::WA_DeleteOnClose);

  menu->addAction("Reset zoom", this, SLOT(resetZoom()));
  menu->addAction("Select new session", this, SLOT(loadSessionChooserPage()));
  menu->popup(ui->customPlot->mapToGlobal(pos));
}


















void MainWindow::loadSessionChooserPage()
{
    localListFiles();
    ui->listWidget->clear();
    for (int i=0; i<localFiles.count();i++)
    {
        if (localFiles[i] != QString(".") && localFiles[i] != QString(".."))
            ui->listWidget->addItem(localFiles[i]);
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::ftpStateChanged(int state)
{
    if (state == QFtp::Connected)
    {
        qDebug() << "Ftp connected.";
        ui->progressBar->setValue(5);
        ftp.login(QString(A20_S4WD_FTP_LOGIN), QString(A20_S4WD_FTP_PASS));
    }

    else if (state == QFtp::LoggedIn)
    {
        qDebug() << "Ftp Logged in.";
        ui->progressBar->setValue(10);
        ftpListFiles();
    }

}

void MainWindow::ftpFileGetProgress(qint64 done, qint64 total)
{
    qDebug() << QString("Ftp get progress %1/%2").arg(done).arg(total);
}

void MainWindow::ftpFinished(int id, bool error)
{
    qDebug() << QString("Ftp finished. Id: %1").arg(id);

    if (id == 4)
    {
        if (newFiles.empty())
        {
            ui->progressBar->setValue(100);
            loadSessionChooserPage();
        }
        else
            ftpGetFile();
    }

    else if (id - 4 == filesUploaded)
    {
        localfile->close();
        delete localfile;


        if (filesUploaded == newFiles.count())
        {
            ftp.close();
            ui->progressBar->setValue(100);
            // go to session chooser page
            loadSessionChooserPage();
        }
        else
        {
            float done = (float)filesUploaded;
            float total = (float)newFiles.count();
            ui->progressBar->setValue((int)(100*(done/total)));
            ftpGetFile();
        }


    }


}

void MainWindow::ftpUrlFound(QUrlInfo url)
{
    if (!localFiles.contains(QString(url.name())))
    {
        qDebug() << QString("New file: %1").arg(url.name());
        newFiles.append(url.name());
    }
    else if (localFiles.contains(QString(url.name())))
    {
        // if file has grown then remove local file and add to list of new files to download
        QFile f(QString(A20_S4WD_LOGGING_FILE).arg(url.name()));
        if (url.size() > f.size())
        {
            qDebug() << QString("File has new data: %1").arg(url.name());
            f.remove();
            newFiles.append(url.name());
            f.close();
        }
    }
}




void MainWindow::ftpSetup()
{
    ui->progressBar->setValue(0);
    connect(&ftp, SIGNAL(stateChanged(int)), this, SLOT(ftpStateChanged(int)));
    connect(&ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpFinished(int,bool)));
    connect(&ftp, SIGNAL(dataTransferProgress(qint64,qint64)), this, SLOT(ftpFileGetProgress(qint64,qint64)));
    connect(&ftp, SIGNAL(listInfo(QUrlInfo)),this,SLOT(ftpUrlFound(QUrlInfo)));
}





void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    //removeAllGraphs();
    // resetchart i.e. remove all axis and plots

    QString filetoload = QString(LOCAL_LOGGING_FILE).arg(ui->listWidget->item(index.row())->text());
    loadData(filetoload,dpDischarge);
    setupAxis();
    addPlot(motorT,drivePlotColors[0],daTemperature);
    addPlot(controllerT,drivePlotColors[1],daTemperature);
    addPlot(volts,drivePlotColors[2],daVolts);
    addPlot(amps,drivePlotColors[3],daAmps);
    addPlot(speed,drivePlotColors[4],daSpeed);
    addPlot(altitude,drivePlotColors[5],daAltitude);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::ftpListFiles()
{
    QString path;
    path = QString(A20_S4WD_LOGGING_DIR);
    newFiles.clear();
    filesUploaded = 0;
    ftp.cd(path);
    ftp.list();
}

void MainWindow::localListFiles()
{
    localFiles.clear();
    QDir dir(QString(LOCAL_LOGGING_DIR));
    qDebug() << dir.dirName();
    localFiles = dir.entryList();
    qDebug() << localFiles;
}

void MainWindow::ftpGetFile()
{
    QString filename = QString(LOCAL_LOGGING_FILE).arg(newFiles[filesUploaded]);
    qDebug() << filename;
    localfile = new QFile(filename);
    localfile->open(QIODevice::WriteOnly);
    ftp.get(QString(A20_S4WD_LOGGING_FILE).arg(newFiles[filesUploaded]),localfile);
    filesUploaded++;
}
























void MainWindow::loadData(QString filepath, DataProfileType profile)
{
    QFile file;
    int i = 0;

    if (profile == dpDischarge)
    {

        motorT.alias = QString("Motor Temperature");
        motorT.datapoints.clear();


        controllerT.alias = QString("Controller Temperature");
        controllerT.datapoints.clear();


        volts.alias = QString("Volts");
        volts.datapoints.clear();


        amps.alias = QString("Amps");
        amps.datapoints.clear();


        speed.alias = QString("Speed");
        speed.datapoints.clear();


        altitude.alias = QString("Altitude");
        altitude.datapoints.clear();

        datetime.clear();
    }

    file.setFileName(filepath);

    double now = QDateTime::currentDateTime().toTime_t();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("Opening file: %1").arg(filepath);
        while (!file.atEnd())
        {
            QString line = file.readLine();
            QStringList tokens = line.split(",");
            if ( profile == dpDischarge )
            {
                if (tokens.count() == 8 )
                {
                    time.append(now + 2*i);

                    //datetime.append(tokens[0]);
                    motorT.datapoints.append(tokens[1].toFloat());
                    controllerT.datapoints.append(tokens[2].toFloat());
                    volts.datapoints.append(tokens[3].toFloat());
                    amps.datapoints.append(tokens[4].toFloat());
                    altitude.datapoints.append(tokens[5].toFloat());
                    speed.datapoints.append(tokens[7].toFloat());

                }
            }
            else if (profile == dpCharge)
            {
                if (tokens.count() == 15)
                {

                }
            }

            i++;

        }
        file.close();
    }




}

void MainWindow::addPlot(Dplot &plot, QColor plotColor, DriveAxisType axisIndex)
{
    ui->customPlot->addGraph(ui->customPlot->xAxis, driveAxis[axisIndex]);
    ui->customPlot->graph()->setData(time, plot.datapoints);
    ui->customPlot->graph()->setName(plot.alias);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot));

    QPen graphPen;
    graphPen.setColor(plotColor);
    graphPen.setWidthF(1);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();
}

void MainWindow::setupAxis()
{
    QVector<double> x;
    for(int i=0; i<datetime.count(); i++)
        x.append(i);

    // delete default x and y axis
    delete ui->customPlot->yAxis;



    // create x axis
    ui->customPlot->xAxis->setLabelColor(QColor(Qt::white));
    ui->customPlot->xAxis->setLabel("Date/time");
    QFont font;
    font.setPointSize(12);
    ui->customPlot->xAxis->setLabelColor(QColor(40,40,40));
    ui->customPlot->xAxis->setLabelFont(font);
    ui->customPlot->xAxis->setAutoTicks(true);
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridVisible(true);
    ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->xAxis->setRange(time[0], time[time.count()-1]);
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);

    ui->customPlot->xAxis->setDateTimeFormat("ddd dd MM\nHH:mm:ss");
    //ui->customPlot->xAxis->setDateTimeSpec(Qt::UTC);
    // set a fixed tick-step to one tick per month:
    //ui->customPlot->xAxis->setAutoTickStep(false);
   // ui->customPlot->xAxis->setTickStep(2628000); // one month in seconds
    //ui->customPlot->xAxis->setSubTickCount(3);
    //qDebug() << QString("Total datetime points: %1").arg(datetime.count());


    // 5 axis (Temperature, Amps, volts, speed, altitude)
    for (int i=0; i<5; i++)
    {
        QCPAxis *baseAxis = new QCPAxis(ui->customPlot->axisRect(),QCPAxis::atLeft);

        baseAxis->setLabelColor(QColor(40,40,40));
        baseAxis->setBasePen(QPen(Qt::white, 1));
        baseAxis->setTickPen(QPen(Qt::white, 1));
        baseAxis->setSubTickPen(QPen(Qt::white, 1));
        baseAxis->setTickLabelColor(Qt::white);
        baseAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
        baseAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        baseAxis->grid()->setSubGridVisible(true);
        baseAxis->grid()->setZeroLinePen(Qt::NoPen);
        baseAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        driveAxis.append(baseAxis);
    }

    driveAxis[0]->setLabel(QString("Temperature"));
    driveAxis[0]->setRangeLower(0);
    driveAxis[0]->setRangeUpper(125);
    ui->customPlot->axisRect()->addAxis(QCPAxis::atLeft, driveAxis[0]);

    driveAxis[1]->setLabel(QString("Amps"));
    driveAxis[1]->setRangeLower(0);
    driveAxis[1]->setRangeUpper(400);
    ui->customPlot->axisRect()->addAxis(QCPAxis::atLeft, driveAxis[1]);

    driveAxis[2]->setLabel(QString("Volts"));
    driveAxis[2]->setRangeLower(42);
    driveAxis[2]->setRangeUpper(60);
    ui->customPlot->axisRect()->addAxis(QCPAxis::atLeft, driveAxis[2]);

    driveAxis[3]->setLabel(QString("Speed"));
    driveAxis[3]->setRangeLower(0);
    driveAxis[3]->setRangeUpper(50);
    ui->customPlot->axisRect()->addAxis(QCPAxis::atLeft, driveAxis[3]);

    driveAxis[4]->setLabel(QString("Altitude"));
    driveAxis[4]->setRangeLower(50);
    driveAxis[4]->setRangeUpper(100);
    ui->customPlot->axisRect()->addAxis(QCPAxis::atLeft, driveAxis[4]);

    qDebug() << "Added axes";

    //ui->customPlot->axisRect()->setupFullAxesBox();
}


void MainWindow::chartSetup()
{

    ui->customPlot->setInteractions(QCP::iRangeDrag /*| QCP::iRangeZoom | QCP::iSelectAxes*/ |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);


    // add title
    ui->customPlot->plotLayout()->insertRow(0);
    QCPPlotTitle *title = new QCPPlotTitle(ui->customPlot,"Drive Profile");
    title->setTextColor(Qt::white);
    QFont font;
    font.setPointSize(15);
    title->setFont(font);
    ui->customPlot->plotLayout()->addElement(0, 0, title);


    ui->customPlot->legend->setBrush(QColor(100,100,100));
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = ui->customPlot->legend->font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setSelectedFont(legendFont);
    ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items


    // styling

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(150,150,150));
    plotGradient.setColorAt(1, QColor(80,80,80));
    ui->customPlot->setBackground(plotGradient);

    ui->customPlot->axisRect()->setBackground(QColor(40,40,40));


    // connect slot that ties some axis selections together (especially opposite axes):
    connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // connect slot that shows a message in the status bar when a graph is clicked:
    //connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

    // setup policy and connect slot for context menu popup:
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
}





void MainWindow::on_syncOkButton_clicked()
{
    localListFiles();
    ftp.connectToHost(QString(A20_S4WD_HOST_IP));
    ui->progressBar->setVisible(true);
}

void MainWindow::on_syncNoButton_clicked()
{
    loadSessionChooserPage();
}
