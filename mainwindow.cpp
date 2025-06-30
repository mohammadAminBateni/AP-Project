#include "mainwindow.h"
#include <QHostInfo>
#include <QList>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QSettings>
#include <QString>
#include <QTimer>
#include <QUdpSocket>
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i < 2; ++i) {
        sockets[i] = new QTcpSocket(this);
        connect(sockets[i], &QTcpSocket::connected, this, &::MainWindow::connectToServer);
        connect(sockets[i], &QTcpSocket::disconnected, this, &MainWindow::handleTcpDisconnected);
    }
    currentServerIp = ui->ip->text();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::connectToServer()
{
    for (int i = 0; i < 2; ++i) {
        if (sockets[i]->state() != QAbstractSocket::ConnectedState) {
            sockets[i]->connectToHost(ui->ip->text(), 8080);
            if (sockets[i]->waitForConnected())
                ui->state->setText("Connected");
            else
                ui->state->setText("Error");
        }
    }
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
void MainWindow::closeAllChildWindows()
{
    const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *widget : topLevelWidgets) {
        if (widget != this) {
            widget->close();
        }
    }
}
