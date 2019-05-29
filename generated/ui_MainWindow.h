/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionShow_Parallel_Coordinates;
    QAction *actionShow_Star_Plots;
    QAction *actionShow_Chernoff_Faces;
    QWidget *centralwidget;
    QLabel *labelTop;
    QProgressBar *progressBar;
    QFrame *line;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1000, 700);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionShow_Parallel_Coordinates = new QAction(MainWindow);
        actionShow_Parallel_Coordinates->setObjectName(QStringLiteral("actionShow_Parallel_Coordinates"));
        actionShow_Star_Plots = new QAction(MainWindow);
        actionShow_Star_Plots->setObjectName(QStringLiteral("actionShow_Star_Plots"));
        actionShow_Chernoff_Faces = new QAction(MainWindow);
        actionShow_Chernoff_Faces->setObjectName(QStringLiteral("actionShow_Chernoff_Faces"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        labelTop = new QLabel(centralwidget);
        labelTop->setObjectName(QStringLiteral("labelTop"));
        labelTop->setGeometry(QRect(20, 20, 1001, 16));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setEnabled(false);
        progressBar->setGeometry(QRect(787, 20, 201, 23));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        line = new QFrame(centralwidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 50, 1011, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(0, 60, 1000, 620));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1000, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuView->addAction(actionShow_Parallel_Coordinates);
        menuView->addAction(actionShow_Star_Plots);
        menuView->addAction(actionShow_Chernoff_Faces);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Visualisierung 1", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open ...", 0));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0));
        actionShow_Parallel_Coordinates->setText(QApplication::translate("MainWindow", "Show Parallel Coordinates", 0));
        actionShow_Star_Plots->setText(QApplication::translate("MainWindow", "Show Star Plots", 0));
        actionShow_Chernoff_Faces->setText(QApplication::translate("MainWindow", "Show Chernoff Faces", 0));
        labelTop->setText(QApplication::translate("MainWindow", "No data loaded", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
