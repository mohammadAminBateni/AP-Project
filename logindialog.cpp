#include "logindialog.h"
#include <QFile>
#include <QMessageBox>
#include <QString>
#include "forgotpassword.h"
#include "ui_logindialog.h"
#include "user.h"
loginDialog::loginDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginDialog)
{
    ui->setupUi(this);
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::on_approve_clicked()
{
    QString password = ui->password->text();
    QString username = ui->username->text();
    QFile f("users.txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open users file!");
        return;
    }
    QTextStream in(&f);
    while (!in.atEnd()) {
        User u(username, password);
        in >> u;
        if (u.getUsername() == username && u.getPassword() == password) {
            //fix this later
            f.close();
            return;
        }
    }
    QMessageBox::warning(
        this,
        "Error",
        "Your username or password is wrong if you forget your password click on forget password!");
    return;
}

void loginDialog::on_forgotPassword_clicked()
{
    forgotPassword fp(this);
    fp.show();
}
