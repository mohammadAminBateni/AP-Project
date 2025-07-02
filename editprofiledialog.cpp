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
}

EditProfileDialog::~EditProfileDialog()
{
    delete ui;
}

void EditProfileDialog::on_approve_clicked()
{
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open users file!");
        return;
    }
    QTextStream stream(&file);
    QStringList allUsers;
    User updatedUser;
    bool userFound = false;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        User u = User::fromString(line);
        if (u.getUsername() == ui->currentUsername->text()) {
            if (!ui->firstName->text().isEmpty())
                u.setFirstName(ui->firstName->text());
            if (!ui->lastName->text().isEmpty())
                u.setLastName(ui->lastName->text());
            if (!ui->phone->text().isEmpty())
                u.setPhone(ui->phone->text());
            if (!ui->gmail->text().isEmpty())
                u.setGmail(ui->gmail->text());
            if (!ui->password->text().isEmpty())
                u.setPassword(QCryptographicHash::hash(ui->password->text().toUtf8(),
                                                       QCryptographicHash::Sha256)
                                  .toHex());
            if (!ui->username->text().isEmpty()
                && ui->username->text() != ui->currentUsername->text()) {
                bool usernameExists = false;
                for (const QString &userLine : allUsers) {
                    User tmp = User::fromString(userLine);
                    if (tmp.getUsername() == ui->username->text()) {
                        usernameExists = true;
                        break;
                    }
                }
                if (usernameExists) {
                    QMessageBox::warning(this, "Error", "Username already exists!");
                    file.close();
                    Menu *m = new Menu(nullptr, ui->currentUsername->text(), socket);
                    m->show();
                    this->close();
                    return;
                }
                u.setUsername(ui->username->text());
            }
            updatedUser = u;
            userFound = true;
        }
        allUsers << u.toString();
    }
    if (!userFound) {
        QMessageBox::warning(this, "Error", "User not found!");
        file.close();
        this->close();
        Menu *m = new Menu(nullptr, ui->currentUsername->text(), socket);
        m->show();
        return;
    }
    file.resize(0);
    QTextStream out(&file);
    for (const QString &line : allUsers) {
        out << line << "\n";
    }
    QString message = "UPDATE_PROFILE:" + ui->currentUsername->text() + ":"
                      + updatedUser.getFirstName() + ":" + updatedUser.getLastName() + ":"
                      + updatedUser.getPhone() + ":" + updatedUser.getGmail() + ":"
                      + updatedUser.getUsername() + ":" + updatedUser.getPassword();
    socket->write(message.toUtf8() + "\n");
    file.close();
    this->close();
    Menu *m = new Menu(nullptr, updatedUser.getUsername(), socket);
    QMessageBox::information(this, "Success", "Profile updated successfully!");
    m->show();
}
