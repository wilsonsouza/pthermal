/********************************************************************************
** Form generated from reading UI file 'pthermalgui.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PTHERMALGUI_H
#define UI_PTHERMALGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PThermalGUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PThermalGUIClass)
    {
        if (PThermalGUIClass->objectName().isEmpty())
            PThermalGUIClass->setObjectName(QStringLiteral("PThermalGUIClass"));
        PThermalGUIClass->resize(600, 400);
        menuBar = new QMenuBar(PThermalGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        PThermalGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PThermalGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PThermalGUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PThermalGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PThermalGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PThermalGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PThermalGUIClass->setStatusBar(statusBar);

        retranslateUi(PThermalGUIClass);

        QMetaObject::connectSlotsByName(PThermalGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *PThermalGUIClass)
    {
        PThermalGUIClass->setWindowTitle(QApplication::translate("PThermalGUIClass", "PThermalGUI", 0));
    } // retranslateUi

};

namespace Ui {
    class PThermalGUIClass: public Ui_PThermalGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PTHERMALGUI_H
