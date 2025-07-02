#include "signupdialog.h"
#include <QFile>
#include <QMessageBox>
#include "login.h"
#include "ui_signupdialog.h"
#include "user.h"
#include <mainwindow.h>

signupDialog::signupDialog(QWidget *parent, QTcpSocket *sock)
    : QWidget(parent)
    , ui(new Ui::signupDialog)
    , socket(sock)
{
    ui->setupUi(this);
    connect(socket, &QTcpSocket::readyRead, this, &signupDialog::onSignupResponse);
}

signupDialog::~signupDialog()
{
    delete ui;
}

QString signupDialog::getName()
{
    return ui->firstName->text();
}
QString signupDialog::getLastName()
{
    return ui->lastName->text();
}
QString signupDialog::getPhone()
{
    return ui->phone->text();
}
QString signupDialog::getGmail()
{
    return ui->gmail->text();
}
QString signupDialog::getUsername()
{
    return ui->username->text();
}
QString signupDialog::getPassword()
{
    return ui->password->text();
}

bool signupDialog::checkPhone()
{
    return ui->phone->text().length() == 11;
}

bool signupDialog::checkGmail()
{
    return ui->gmail->text().contains("@gmail.com") || ui->gmail->text().contains("@yahoo.com");
}

bool signupDialog::checkPassword()
{
    return ui->password->text().length() >= 8;
}

void signupDialog::on_approve_clicked()
{
    if (!checkGmail() || !checkPassword() || !checkPhone()) {
        QMessageBox::warning(this,
                             "Warning",
                             "Your email address, phone number or password is invalid!");
        return;
    }

    User u(ui->firstName->text(),
           ui->lastName->text(),
           ui->phone->text(),
           ui->gmail->text(),
           ui->username->text(),
           ui->password->text());

    QFile f("users.txt");
    if (!f.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "The file cannot be opened!");
        return;
    }

    QTextStream out(&f);
    out << u;
    f.close();

    QString message = "SIGNUP:" + u.getFirstName() + ":" + u.getLastName() + ":" + u.getUsername()
                      + ":" + u.getPassword() + ":" + u.getPhone() + ":" + u.getGmail();
    socket->write(message.toUtf8() + "\n");
}

void signupDialog::onSignupResponse()
{
    qDebug() << "Server response received!";
    QString response = QString::fromUtf8(socket->readAll());
    if (response.trimmed() == "SIGNUP_SUCCESS") {
        QMessageBox::information(this, "Success", "Signup completed successfully!");
        this->close();
        login *l = new login(nullptr, socket);
        l->show();
    } else if (response.startsWith("SIGNUP_FAILED")) {
        QMessageBox::warning(this, "Signup Failed", "Signup failed! Username may already exist.");
    }
}
