#include "editprofiledialog.h"
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "ui_editprofiledialog.h"
#include "user.h"
#include <mainwindow.h>
EditProfileDialog::EditProfileDialog(QWidget *parent, QTcpSocket *sock)
    : QWidget(parent)
    , ui(new Ui::EditProfileDialog)
    , socket(sock)
{
    ui->setupUi(this);
    connect(socket, &QTcpSocket::readyRead, this, &EditProfileDialog::readyRead);
}

EditProfileDialog::~EditProfileDialog()
{
    delete ui;
}

void EditProfileDialog::readyRead()
{
    QString response = QString::fromUtf8(socket->readAll()).trimmed();
    if (response == "UPDATE_SUCCESS") {
        QMessageBox::information(this, "Success", "Your info has changed successfully");
        this->close();
        Menu *m = new Menu(nullptr, updatedUser.getUsername(), socket);
        m->show();
    } else {
        QMessageBox::warning(this, "Error", "An error has occured");
        this->close();
        Menu *m = new Menu(nullptr, updatedUser.getUsername(), socket);
        m->show();
    }
}

void EditProfileDialog::on_approve_clicked()
{
    if (!ui->firstName->text().isEmpty()) {
        updatedUser.setFirstName(ui->firstName->text());
    }
    if (!ui->username->text().isEmpty()) {
        updatedUser.setUsername(ui->username->text());
    }
    if (!ui->gmail->text().isEmpty()) {
        updatedUser.setGmail(ui->gmail->text());
    }
    if (!ui->lastName->text().isEmpty()) {
        updatedUser.setLastName(ui->lastName->text());
    }
    if (!ui->phone->text().isEmpty()) {
        updatedUser.setPhone(ui->phone->text());
    }
    if (!ui->password->text().isEmpty()) {
        if (ui->password->text().length() >= 8) {
            updatedUser.setPassword(ui->password->text());
        }
    }
    QString message = "UPDATE_PROFILE:" + ui->currentUsername->text() + ":"
                      + updatedUser.getFirstName() + ":" + updatedUser.getLastName() + ":"
                      + updatedUser.getPhone() + ":" + updatedUser.getGmail() + ":"
                      + updatedUser.getUsername() + ":" + updatedUser.getPassword();
    socket->write(message.toUtf8() + "\n");
}
