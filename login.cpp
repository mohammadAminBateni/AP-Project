#include "login.h"
#include "logindialog.h"
#include "signupdialog.h"
#include "ui_login.h"
login::login(QWidget *parent, QTcpSocket *sock)
    : QWidget(parent)
    , ui(new Ui::login)
    , socket(sock)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_signup_clicked()
{
    signupDialog *s = new signupDialog(nullptr, socket);
    s->show();
    this->close();
}

void login::on_login_2_clicked()
{
    loginDialog *l = new loginDialog(nullptr, socket);
    l->show();
    this->close();
}
