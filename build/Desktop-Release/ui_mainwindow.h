/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QPushButton *start_Button;
    QLabel *ipLabel;
    QTextEdit *logBox;
    QLineEdit *portEdit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1267, 498);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        start_Button = new QPushButton(centralwidget);
        start_Button->setObjectName("start_Button");

        verticalLayout->addWidget(start_Button);

        ipLabel = new QLabel(centralwidget);
        ipLabel->setObjectName("ipLabel");

        verticalLayout->addWidget(ipLabel);

        logBox = new QTextEdit(centralwidget);
        logBox->setObjectName("logBox");

        verticalLayout->addWidget(logBox);

        portEdit = new QLineEdit(centralwidget);
        portEdit->setObjectName("portEdit");

        verticalLayout->addWidget(portEdit);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        start_Button->setText(QCoreApplication::translate("MainWindow", "Start Server", nullptr));
        ipLabel->setText(QCoreApplication::translate("MainWindow", "IP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
