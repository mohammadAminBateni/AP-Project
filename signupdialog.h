#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QWidget>
#include "qtcpsocket.h"

namespace Ui {
class signupDialog;
}

class signupDialog : public QWidget
{
    Q_OBJECT

public:
    explicit signupDialog(QWidget *parent = nullptr,QTcpSocket*socket=nullptr);
    ~signupDialog();
    QString getName();
    QString getLastName();
    QString getPhone();
    QString getGmail();
    QString getUsername();
    QString getPassword();
    bool checkPhone();
    bool checkGmail();

    bool checkPassword();
private slots:
    void on_approve_clicked();

private:
    Ui::signupDialog *ui;
    QTcpSocket *socket;
};

#endif // SIGNUPDIALOG_H
