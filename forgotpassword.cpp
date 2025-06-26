#include "forgotpassword.h"
#include <QByteArray>
#include <QFile>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_forgotpassword.h"
#include "user.h"
forgotPassword::forgotPassword(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::forgotPassword)
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
    QByteArray block;
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
        f.resize(0); // پاک کردن فایل
        QTextStream out(&f);
        for (const QString &line : lines) {
            out << line << "\n";
        }
        QMessageBox::information(this, "Success", "Password updated!");
    } else {
        QMessageBox::warning(this, "Error", "Phone number not found!");
    }
    block.append(f.readAll());
    MainWindow m;
    m.getSocket1()->write(block);
    m.getSocket2()->write(block);
    f.flush();
    f.close();
}
