#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_Button_clicked();

private:
    Ui::MainWindow *ui;
    Server* server = nullptr;

    QString getLocalIpAddress();
    void log(const QString& message);
};

#endif // MAINWINDOW_H
