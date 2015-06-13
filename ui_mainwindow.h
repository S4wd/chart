/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "tgchart.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QStackedWidget *stackedWidget;
    QWidget *syncPage;
    QProgressBar *progressBar;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *syncLabel;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *syncOkButton;
    QPushButton *syncNoButton;
    QWidget *sessionChooserPage;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QWidget *plotPage;
    QHBoxLayout *horizontalLayout;
    TgChart *customPlot;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(953, 707);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(80, 80, 80);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        titleLabel = new QLabel(centralWidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 87 12pt \"Arial\";"));

        verticalLayout->addWidget(titleLabel);

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        syncPage = new QWidget();
        syncPage->setObjectName(QString::fromUtf8("syncPage"));
        progressBar = new QProgressBar(syncPage);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(310, 280, 321, 23));
        progressBar->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        progressBar->setValue(24);
        frame = new QFrame(syncPage);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(360, 160, 191, 102));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        syncLabel = new QLabel(frame);
        syncLabel->setObjectName(QString::fromUtf8("syncLabel"));
        syncLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 10pt \"Arial\";"));

        verticalLayout_2->addWidget(syncLabel);

        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        syncOkButton = new QPushButton(frame_2);
        syncOkButton->setObjectName(QString::fromUtf8("syncOkButton"));
        syncOkButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_3->addWidget(syncOkButton);

        syncNoButton = new QPushButton(frame_2);
        syncNoButton->setObjectName(QString::fromUtf8("syncNoButton"));
        syncNoButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_3->addWidget(syncNoButton);


        verticalLayout_2->addWidget(frame_2);

        stackedWidget->addWidget(syncPage);
        sessionChooserPage = new QWidget();
        sessionChooserPage->setObjectName(QString::fromUtf8("sessionChooserPage"));
        horizontalLayout_2 = new QHBoxLayout(sessionChooserPage);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        listWidget = new QListWidget(sessionChooserPage);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_2->addWidget(listWidget);

        stackedWidget->addWidget(sessionChooserPage);
        plotPage = new QWidget();
        plotPage->setObjectName(QString::fromUtf8("plotPage"));
        horizontalLayout = new QHBoxLayout(plotPage);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        customPlot = new TgChart(plotPage);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(customPlot);

        stackedWidget->addWidget(plotPage);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Solar Desert Challenge Charting", 0, QApplication::UnicodeUTF8));
        titleLabel->setText(QApplication::translate("MainWindow", "Solar 4WD Challenge Data Viewer", 0, QApplication::UnicodeUTF8));
        syncLabel->setText(QApplication::translate("MainWindow", "Download latest data ?", 0, QApplication::UnicodeUTF8));
        syncOkButton->setText(QApplication::translate("MainWindow", "Yes", 0, QApplication::UnicodeUTF8));
        syncNoButton->setText(QApplication::translate("MainWindow", "No", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
