#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->logBox->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLocalIpAddress()
{
    const QList<QHostAddress>& list = QNetworkInterface::allAddresses();
    for (const QHostAddress& addr : list)
    {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol &&
            addr != QHostAddress::LocalHost)
        {
            return addr.toString();
        }
    }
    return QHostAddress(QHostAddress::LocalHost).toString();
}

void MainWindow::log(const QString& message)
{
    ui->logBox->append(message);
}

void MainWindow::on_start_Button_clicked()
{
    bool ok;
    quint16 port = ui->portEdit->text().toUShort(&ok);
    if (!ok || port == 0)
    {
        QMessageBox::warning(this, "Error", "Please enter a valid port number.");
        return;
    }

    server = new Server(this);
    server->startServer(port);

    QString ip = getLocalIpAddress();
    ui->ipLabel->setText("IP: " + ip);
    log("Server started on port " + QString::number(port));
}

