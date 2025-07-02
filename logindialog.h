#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QWidget>
#include "qtcpsocket.h"

namespace Ui {
class loginDialog;
}

class loginDialog : public QWidget
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = nullptr, QTcpSocket *socket = nullptr);
    ~loginDialog();

public slots:
    void onServerResponse();
private slots:

    void on_approve_clicked();

    void on_forgotPassword_clicked();

private:
    Ui::loginDialog *ui;
    QTcpSocket *socket;
};

#endif // LOGINDIALOG_H
