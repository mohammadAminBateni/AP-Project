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
}

forgotPassword::~forgotPassword()
{
    delete ui;
}

QString forgotPassword::getPhone()
{
    return ui->phone->text();
}

void forgotPassword::on_approve_clicked()
{
    if (ui->newPassword->text().length() < 8) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters!");
        return;
    }

    QFile f("users.txt");
    if (!f.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open users file!");
        return;
    }

    QTextStream in(&f);
    QStringList lines;
    bool userFound = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() >= 6 && parts[3] == ui->phone->text()) {
            User tempUser;
            QString hashedPassword = tempUser.hashPassword(ui->newPassword->text());
            parts[5] = hashedPassword;
            line = parts.join("|");
            userFound = true;
        }
        lines.append(line);
    }

    if (userFound) {
        f.resize(0);
        QTextStream out(&f);
        for (const QString &line : lines) {
            out << line << "\n";
        }
        QString newPasswordHashed = User().hashPassword(ui->newPassword->text());
        QString msg = "PASSWORD_RESET:" + ui->phone->text() + ":" + newPasswordHashed;
        socket->write(msg.toUtf8() + "\n");

        QMessageBox::information(this, "Success", "Password updated!");
        this->close();
        login *l = new login(nullptr, socket);
        l->show();
    } else {
        QMessageBox::warning(this, "Error", "Phone number not found!");
    }

    f.close();
}
