#include "mainwindow.h"
#include <QHostInfo>
#include <QList>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QSettings>
#include <QString>
#include <QTimer>
#include <QUdpSocket>
#include "login.h"
#include "signupdialog.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &::MainWindow::on_connect_clicked);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::handleTcpDisconnected);

    currentServerIp = ui->ip->text();
}
MainWindow::~MainWindow()
{
    delete ui;
}
// void MainWindow::connectToServer()
// {
//     if (socket->state() != QAbstractSocket::ConnectedState) {
//         socket->connectToHost(ui->ip->text(), 8080);
//         if (socket->waitForConnected()) {
//             ui->state->setText("Connected");
//             signupDialog *sd = new signupDialog(this,socket);
//             sd->show();
//         } else
//             ui->state->setText("Error");
//     }
// }
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

void MainWindow::on_connect_clicked()
{
    if (ui->ip->text().trimmed().isEmpty()) {
        ui->state->setText("Please enter server IP");
        return;
    }

    if (socket->state() != QAbstractSocket::ConnectedState) {
        socket->connectToHost(ui->ip->text(), 8080);
        if (socket->waitForConnected()) {
            ui->state->setText("Connected");
            login *l = new login(nullptr, socket);
            this->hide();
            l->show();
        } else
            ui->state->setText("Error");
    }
}
