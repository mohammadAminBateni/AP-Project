#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
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
public slots:
    void clientConnected();
    void readyRead();
    void bytesWritten();
    void disconnect();

private:
    Ui::MainWindow *ui;
    QTcpSocket *sockets[2];
};
#endif // MAINWINDOW_H
