#include "login.h"
#include "logindialog.h"
#include "signupdialog.h"
#include "ui_login.h"
login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_signup_clicked()
{
    signupDialog s(this);
    s.show();
}

void login::on_login_2_clicked()
{
    loginDialog l(this);
    l.show();
}
