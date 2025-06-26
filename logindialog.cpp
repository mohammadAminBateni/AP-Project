#include "logindialog.h"
#include <QFile>
#include <QMessageBox>
#include <QString>
#include "forgotpassword.h"
#include "mainwindow.h"
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
    User tempUser;
    QString hashedPassword = tempUser.hashPassword(ui->password->text());
    QString username = ui->username->text();
    QString password = hashedPassword;
    QByteArray block;
    QFile f("users.txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open users file!");
        return;
    }

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() >= 6 && parts[4] == username && parts[5] == password) {
            QMessageBox::information(this, "Success", "Login successful!");
            block.append(f.readAll());
            MainWindow m;
            m.getSocket1()->write(block);
            m.getSocket2()->write(block);
            f.flush();
            f.close();
            f.close();
            return;
        }
    }

    QMessageBox::warning(this, "Error", "Invalid username or password!");
    f.close();
}

void loginDialog::on_forgotPassword_clicked()
{
    forgotPassword fp(this);
    fp.show();
}
