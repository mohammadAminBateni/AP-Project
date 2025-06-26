#include "mainwindow.h"
#include <QNetworkDatagram>
#include <QSettings>
#include <QUdpSocket>
#include "forgotpassword.h"
#include "login.h"
#include "menu.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress); // پورت دلخواه (باید با سرور یکی باشه)
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::processBroadcast);
    connect(menu, &Menu::logoutRequest, this, &MainWindow::handleLogout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processBroadcast()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString serverIp = datagram.senderAddress().toString();

        for (int i = 0; i < 2; ++i) {
            sockets[i] = new QTcpSocket(this);
            sockets[i]->connectToHost(serverIp, 8080); // پورت TCP سرور
            connect(sockets[i], &QTcpSocket::readyRead, this, &MainWindow::readyRead);
            connect(sockets[i], &QTcpSocket::bytesWritten, this, &MainWindow::bytesWritten);
            connect(sockets[i], &QTcpSocket::disconnected, this, &MainWindow::disconnect);
        }
    }
}

void MainWindow::readyRead() {}

void MainWindow::bytesWritten() {}

void MainWindow::disconnect() {}

void MainWindow::handleLogout()
{
    closeAllChildWindows();

    QSettings settings;
    settings.remove("session_token");
    settings.remove("last_login");
    login *l = new login;
    l->show();
}
void MainWindow::closeAllChildWindows()
{
    const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *widget : topLevelWidgets) {
        if (widget != this) {
            widget->close();
        }
    }
}
QTcpSocket *MainWindow::getSocket1()
{
    return sockets[0];
}
QTcpSocket *MainWindow::getSocket2()
{
    return sockets[1];
}
