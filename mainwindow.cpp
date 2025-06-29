#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString ip = getLocalIPAddress();
    qDebug() << "Local IP:" << ip;
    ui->labelLocalIP->setText("Local IP: " + ip);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLocalIPAddress()
{
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces())
    {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {

            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
            {
                QHostAddress ip = entry.ip();
                if (ip.protocol() == QAbstractSocket::IPv4Protocol)
                    return ip.toString();
            }
        }
    }
    return "127.0.0.1";
}
