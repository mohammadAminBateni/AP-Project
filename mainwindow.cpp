#include "mainwindow.h"
#include <QHostInfo>
#include <QList>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QSettings>
#include <QString>
#include <QTimer>
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
    for (int i = 0; i < 2; ++i) {
        sockets[i] = new QTcpSocket(this);
        connect(sockets[i], &QTcpSocket::connected, this, &MainWindow::handleTcpConnected);
        connect(sockets[i], &QTcpSocket::disconnected, this, &MainWindow::handleTcpDisconnected);
        connect(menu, &Menu::logoutRequest, this, &MainWindow::handleLogout);
    }
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

        // Validate the broadcast message
        if (datagram.data() == "GAME_SERVER_DISCOVERY") {
            if (serverIp != currentServerIp) {
                currentServerIp = serverIp;
                connectToServer(serverIp);
            }
        }
    }
}
void MainWindow::connectToServer(const QString &ip)
{
    for (int i = 0; i < 2; ++i) {
        if (sockets[i]->state() != QAbstractSocket::ConnectedState) {
            sockets[i]->connectToHost(ip, 8080);
        }
    }
}
void MainWindow::handleTcpConnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
}
void MainWindow::readyRead() {}

void MainWindow::bytesWritten() {}

void MainWindow::handleTcpDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    qDebug() << "TCP connection lost!";
    QTimer::singleShot(5000, [this, socket]() {
        if (!currentServerIp.isEmpty()) {
            socket->connectToHost(currentServerIp, 8080);
        }
    });
}

void MainWindow::handleLogout()
{
    closeAllChildWindows();

    QSettings settings;
    settings.remove("session_token");
    settings.remove("last_login");
    this->close();
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
