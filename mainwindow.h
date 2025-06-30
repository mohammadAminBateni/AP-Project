#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>
#include "menu.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeAllChildWindows();
public slots:
    void readyRead();
    void bytesWritten();
    void disconnect();
    void handleTcpDisconnected();

private:
    Ui::MainWindow *ui;
    QTcpSocket *sockets[2];
    Menu *menu;
    QString currentServerIp;
    void connectToServer();
};
#endif // MAINWINDOW_H
