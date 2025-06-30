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
        QMessageBox::information(this, "Success", "Login successful!");
        QString message = "LOGIN:" + username + ":" + hashedPassword;
        socket->write(message.toUtf8());
        Menu *menu = new Menu(nullptr, username, socket);
        menu->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Error", "Invalid username or password!");
    }
}

void loginDialog::on_forgotPassword_clicked()
{
    forgotPassword fp(this);
    fp.show();
    this->close();
}
