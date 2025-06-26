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
    QTcpSocket *getSocket1();
    QTcpSocket *getSocket2();
    void closeAllChildWindows();
public slots:
    void processBroadcast();
    void readyRead();
    void bytesWritten();
    void disconnect();
    void handleLogout();

private:
    Ui::MainWindow *ui;
    QTcpSocket *sockets[2];
    QUdpSocket *udpSocket;
    Menu *menu;
};
#endif // MAINWINDOW_H
