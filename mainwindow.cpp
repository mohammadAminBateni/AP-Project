#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i < 2; i++) {
        connect(sockets[i], &QTcpSocket::connected, this, &MainWindow::clientConnected);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clientConnected()
{
    for (int i = 0; i < 2; i++) {
        sockets[i] = new QTcpSocket();
        sockets[i]->connectToHost(, 8080);
        connect(sockets[i], &QTcpSocket::readyRead, this, &MainWindow::readyRead);
        connect(sockets[i], &QTcpSocket::bytesWritten, this, &MainWindow::bytesWritten);
        connect(sockets[i], &QTcpSocket::disconnected, this, &MainWindow::disconnect);
    }
}

void MainWindow::readyRead() {}

void MainWindow::bytesWritten() {}

void MainWindow::disconnect() {}
