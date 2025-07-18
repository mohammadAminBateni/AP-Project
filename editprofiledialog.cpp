#include "editprofiledialog.h"
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include "ui_editprofiledialog.h"
#include "user.h"
#include <mainwindow.h>
#include "menu.h"

EditProfileDialog::EditProfileDialog(QWidget *parent, QTcpSocket *sock, const QString &username)
    : QWidget(parent)
    , ui(new Ui::EditProfileDialog)
    , socket(sock)
    , currentUsername(username)
{
    ui->setupUi(this);
    connect(socket, &QTcpSocket::readyRead, this, &EditProfileDialog::readyRead);
    
    // Set the current username in the UI
    if (ui->currentUsername) {
        ui->currentUsername->setText(currentUsername);
    }
}

EditProfileDialog::~EditProfileDialog()
{
    delete ui;
}

void EditProfileDialog::setCurrentUser(const User &user)
{
    updatedUser = user;
    currentUsername = user.getUsername();
    
    // Pre-fill the form with current user data
    if (ui->currentUsername) {
        ui->currentUsername->setText(currentUsername);
    }
}

void EditProfileDialog::readyRead()
{
    QString response = QString::fromUtf8(socket->readAll()).trimmed();
    
    // Add debug output to see what response we're getting
    qDebug() << "Received response:" << response;
    
    if (response == "UPDATE_SUCCESS") {
        QMessageBox::information(this, "Success", "Your info has changed successfully");
        this->close();
        Menu *m = new Menu(nullptr, updatedUser.getUsername().isEmpty() ? currentUsername : updatedUser.getUsername(), socket);
        m->show();
    } else {
        // Show more detailed error message
        QString errorMsg = "An error has occurred";
        if (response.startsWith("UPDATE_FAIL:")) {
            errorMsg = response.mid(12); // Remove "UPDATE_FAIL:" prefix
        }
        QMessageBox::warning(this, "Error", errorMsg);
        this->close();
        Menu *m = new Menu(nullptr, currentUsername, socket);
        m->show();
    }
}

void EditProfileDialog::on_approve_clicked()
{
    // Get the current username from UI or use the stored one
    QString oldUsername = currentUsername;
    if (ui->currentUsername && !ui->currentUsername->text().isEmpty()) {
        oldUsername = ui->currentUsername->text();
    }
    
    // Initialize updatedUser with current values if not set
    if (updatedUser.getUsername().isEmpty()) {
        updatedUser.setUsername(oldUsername);
    }
    
    // Update only non-empty fields
    QString newFirstName = updatedUser.getFirstName();
    QString newLastName = updatedUser.getLastName();
    QString newPhone = updatedUser.getPhone();
    QString newEmail = updatedUser.getGmail();
    QString newUsername = updatedUser.getUsername();
    QString newPassword = updatedUser.getPassword();
    
    if (!ui->firstName->text().isEmpty()) {
        newFirstName = ui->firstName->text();
        updatedUser.setFirstName(newFirstName);
    }
    if (!ui->lastName->text().isEmpty()) {
        newLastName = ui->lastName->text();
        updatedUser.setLastName(newLastName);
    }
    if (!ui->phone->text().isEmpty()) {
        newPhone = ui->phone->text();
        updatedUser.setPhone(newPhone);
    }
    if (!ui->gmail->text().isEmpty()) {
        newEmail = ui->gmail->text();
        updatedUser.setGmail(newEmail);
    }
    if (!ui->username->text().isEmpty()) {
        newUsername = ui->username->text();
        updatedUser.setUsername(newUsername);
    }
    if (!ui->password->text().isEmpty()) {
        if (ui->password->text().length() >= 8) {
            newPassword = ui->password->text();
            updatedUser.setPassword(newPassword);
        }
    }
    
    // Build the message with proper format - ensure no empty fields
    QString message = QString("UPDATE_PROFILE:%1:%2:%3:%4:%5:%6:%7")
                      .arg(oldUsername)
                      .arg(newFirstName.isEmpty() ? "" : newFirstName)
                      .arg(newLastName.isEmpty() ? "" : newLastName)
                      .arg(newPhone.isEmpty() ? "" : newPhone)
                      .arg(newEmail.isEmpty() ? "" : newEmail)
                      .arg(newUsername.isEmpty() ? oldUsername : newUsername)
                      .arg(newPassword.isEmpty() ? "" : newPassword);
    
    qDebug() << "Sending message:" << message;
    socket->write(message.toUtf8() + "\n");
}
