#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  srand(QDateTime::currentDateTime().toTime_t());
  ui->setupUi(this);

  ui->customPlot->setInteractions(QCP::iRangeDrag /*| QCP::iRangeZoom*/ | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);


  ui->customPlot->plotLayout()->insertRow(0);
  QCPPlotTitle *title = new QCPPlotTitle(ui->customPlot,"Solar Desert Challenge");
  title->setTextColor(Qt::white);
  QFont font;
  font.setPointSize(15);
  title->setFont(font);
  ui->customPlot->plotLayout()->addElement(0, 0, title);


  ui->customPlot->xAxis->setLabelColor(QColor(Qt::white));
  ui->customPlot->yAxis->setLabelColor(QColor(Qt::white));
  ui->customPlot->xAxis->setLabel("Date/time");
  ui->customPlot->yAxis->setLabel("Temperature");

  ui->customPlot->legend->setVisible(true);
  QFont legendFont = ui->customPlot->legend->font();
  legendFont.setPointSize(10);
  ui->customPlot->legend->setFont(legendFont);
  ui->customPlot->legend->setSelectedFont(legendFont);
  ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items



  ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
  ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
  ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
  ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
  ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->customPlot->xAxis->setTickLabelColor(Qt::white);
  ui->customPlot->yAxis->setTickLabelColor(Qt::white);
  ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  ui->customPlot->xAxis->grid()->setSubGridVisible(true);
  ui->customPlot->yAxis->grid()->setSubGridVisible(true);
  ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
  ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
  ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
  ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(80, 80, 80));
  plotGradient.setColorAt(1, QColor(50, 50, 50));
  ui->customPlot->setBackground(plotGradient);
  QLinearGradient axisRectGradient;
  axisRectGradient.setStart(0, 0);
  axisRectGradient.setFinalStop(0, 350);
  axisRectGradient.setColorAt(0, QColor(140, 140, 140));
  axisRectGradient.setColorAt(1, QColor(80, 80, 80));
  ui->customPlot->axisRect()->setBackground(axisRectGradient);


  connect(&ftp, SIGNAL(stateChanged(int)), this, SLOT(ftpStateChanged(int)));
  //connect(&ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpFinished(int,bool)));
  connect(&ftp, SIGNAL(dataTransferProgress(qint64,qint64)), this, SLOT(ftpFileGetProgress(qint64,qint64)));


  loadData();
  addRandomGraph();

  ui->customPlot->xAxis->setRange(0, temperature.datapoints.count());
  ui->customPlot->yAxis->setRange(10, 50);
  ui->customPlot->axisRect()->setupFullAxesBox();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();

  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // connect some interaction slots:
  connect(ui->customPlot, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)), this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
  connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

  // setup policy and connect slot for context menu popup:
  ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title)
{
  Q_UNUSED(event)
  // Set the plot title by double clicking on it
  bool ok;
  QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
  if (ok)
  {
    title->setText(newTitle);
    ui->customPlot->replot();
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}

void MainWindow::mousePress()
{
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
}

void MainWindow::mouseWheel()
{
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
}

void MainWindow::addRandomGraph()
{

  QVector<double> x;

  for(int i=0; i<temperature.datapoints.count(); i++)
      x.append(i);

  QCPGraph * graph = ui->customPlot->addGraph();
  ui->customPlot->graph()->setName(temperature.alias);
  ui->customPlot->graph()->setData(x, temperature.datapoints);
  ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);

  ui->customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot));
  QPen graphPen;
  graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
  graphPen.setWidthF(1);
  ui->customPlot->graph()->setPen(graphPen);
  ui->customPlot->replot();

  ui->customPlot->setTracerGraph(graph);
}

void MainWindow::removeSelectedGraph()
{
  if (ui->customPlot->selectedGraphs().size() > 0)
  {
    ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
    ui->customPlot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
}

void MainWindow::resetZoom()
{
   ui->customPlot->xAxis->setRange(0, temperature.datapoints.count());
   ui->customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setStyleSheet("QMenu { font-size:11px; width: 150px; color:white; left: 20px; background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #cccccc, stop: 1 #333333);}");
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->customPlot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->customPlot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
    if (ui->customPlot->graphCount() > 0)
      menu->addAction("Reset zoom", this, SLOT(resetZoom()));

    menu->addAction("Retrieve data", this, SLOT(DownloadDataHandler()));
  }

  menu->popup(ui->customPlot->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable)
{
    ui->statusBar->showMessage(QString("Clicked on graph '%1'.").arg(plottable->name()), 1000);
}

void MainWindow::DownloadDataHandler()
{
    progressdialog.setLabelText("Close");
    progressdialog.setWindowModality(Qt::WindowModal);
    progressdialog.setRange(0,10);
    connect(&progressdialog, SIGNAL(canceled()), this, SLOT(progressdialogCancelled()));

    ftp.connectToHost("192.168.40.1");
}

void MainWindow::ftpStateChanged(int state)
{
    if (state == QFtp::Connected)
    {
        qDebug() << "Ftp connected.";
        ftp.login(QString("tgard"), QString("vaughn"));
    }

    else if (state == QFtp::LoggedIn)
    {
        qDebug() << "Ftp Logged in.";
        progressdialog.setValue(20);
        localfile = new QFile(QString(CSV_FILE).arg(7));
        localfile->open(QIODevice::WriteOnly);
        ftp.cd(QString("/home/s4wd/logging/"));
        ftp.get(QString("7s4wd.log"),localfile);

    }

}

void MainWindow::ftpFileGetProgress(qint64 done, qint64 total)
{
    qDebug() << QString("Ftp get progress %1/%2").arg(done).arg(total);
    progressdialog.setValue(progressdialog.value()+1);
}

void MainWindow::ftpFinished(int id, bool error)
{
    //qDebug() << "Ftp finished.";
    //ftp.close();
}

void MainWindow::progressdialogCancelled()
{
    ftp.close();
    localfile->close();
}

qint16 MainWindow::countDataFiles()
{
    QDir dir(DATA_DIR);
    QStringList filters;
    filters << "*nrd";
    return dir.entryList(filters, QDir::Filters(QDir::Files)).count();
}

void MainWindow::loadData()
{
    QString filename;
    QFile file;

    temperature.alias = QString("Temperature");
    temperature.datapoints.clear();
    datetime.clear();

    qint16 dataFileCount = countDataFiles();
    for (int i=0; i<dataFileCount; i++)
    {
        filename = QString(DATA_FILE).arg(i);
        file.setFileName(filename);
        qDebug() << QString("Opening file: %1").arg(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                QStringList tokens = line.split(",");
                if (tokens.count() == 6)
                {
                    datetime.append(tokens[0]);
                    temperature.datapoints.append(tokens[2].toFloat());
                }
            }
            file.close();
        }
    }
}



