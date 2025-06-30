#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "qtcpsocket.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr, QTcpSocket *socket = nullptr);
    ~login();

private slots:
    void on_signup_clicked();

    void on_login_2_clicked();

private:
    Ui::login *ui;
    QTcpSocket *socket;
};

#endif // LOGIN_H
