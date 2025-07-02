#include "logindialog.h"
#include <QFile>
#include <QMessageBox>
#include <QString>
#include "forgotpassword.h"
#include "menu.h"
#include "ui_logindialog.h"
#include "user.h"
loginDialog::loginDialog(QWidget *parent, QTcpSocket *sock)
    : QWidget(parent)
    , ui(new Ui::loginDialog)
    , socket(sock)
{
    ui->setupUi(this);
    connect(socket, &QTcpSocket::readyRead, this, &loginDialog::onServerResponse);
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::on_approve_clicked()
{
    QString username = ui->username->text().trimmed();
    QString password = ui->password->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter both username and password.");
        return;
    }

    User tempUser;
    QString hashedPassword = tempUser.hashPassword(password);

    QFile f("users.txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open users file!");
        return;
    }

    QTextStream in(&f);
    bool found = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() >= 6) {
            QString storedUsername = parts[4].trimmed();
            QString storedPassword = parts[5].trimmed();

            if (storedUsername == username && storedPassword == hashedPassword) {
                found = true;
                break;
            }
        }
    }
    f.close();

    if (found) {
        QString message = "LOGIN:" + username + ":" + hashedPassword;
        socket->write(message.toUtf8() + "\n");
        onServerResponse();
    }
}
void loginDialog::onServerResponse()
{
    {
        QString response = QString::fromUtf8(socket->readAll()).trimmed();

        if (response == "LOGIN_SUCCESS") {
            QMessageBox::information(this, "Success", "Login successful!");
            Menu *menu = new Menu(nullptr, ui->username->text().trimmed(), socket);
            menu->show();
            this->close();
        } else if (response.startsWith("LOGIN_FAIL")) {
            QMessageBox::warning(this, "Login Failed", "Invalid username or password!");
        }
    }
}
void loginDialog::on_forgotPassword_clicked()
{
    forgotPassword *fp = new forgotPassword(nullptr, socket);
    fp->show();
    this->close();
}
