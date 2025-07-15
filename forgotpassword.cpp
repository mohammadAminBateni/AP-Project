#include "forgotpassword.h"
#include <QByteArray>
#include <QFile>
#include <QMessageBox>
#include "login.h"
#include "ui_forgotpassword.h"
#include "user.h"
forgotPassword::forgotPassword(QWidget *parent, QTcpSocket *sock)
    : QWidget(parent)
    , ui(new Ui::forgotPassword)
    , socket(sock)
{
    ui->setupUi(this);
    connect(socket, &QTcpSocket::readyRead, this, &forgotPassword::readyRead);
}

forgotPassword::~forgotPassword()
{
    delete ui;
}

QString forgotPassword::getPhone()
{
    return ui->phone->text();
}

void forgotPassword::readyRead()
{
    QString response = QString::fromUtf8(socket->readAll()).trimmed();
    if (response == "PASSWORD_RESET_SUCCESS") {
        QMessageBox::information(this, "Success", "Password has successfully changed");
        this->close();
        login *l = new login(nullptr, socket);
        l->show();
    } else {
        QMessageBox::warning(this, "Error", "The Phone Number is wrong");
        this->close();
        login *l = new login(nullptr, socket);
        l->show();
    }
}

void forgotPassword::on_approve_clicked()
{
    if (ui->newPassword->text().length() < 8) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters!");
        return;
    }
    QString newPasswordHashed = User().hashPassword(ui->newPassword->text());
    QString msg = "PASSWORD_RESET:" + ui->phone->text() + ":" + newPasswordHashed;
    socket->write(msg.toUtf8() + "\n");
}
