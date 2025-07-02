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
    void handleTcpDisconnected();

private slots:
    void on_connect_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    Menu *menu;
    QString currentServerIp;
};
#endif // MAINWINDOW_H
