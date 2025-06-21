#include "signupdialog.h"
#include <QFile>
#include <QMessageBox>
#include "ui_signupdialog.h"
#include "user.h"
signupDialog::signupDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signupDialog)
{
    ui->setupUi(this);
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
    if (ui->phone->text().length() == 11) {
        return true;
    }
    return false;
}

bool signupDialog::checkGmail()
{
    if (ui->gmail->text().contains("@gmail.com") || ui->gmail->text().contains("@yahoo.com")) {
        return true;
    }
    return false;
}
bool signupDialog::checkPassword()
{
    if (ui->password->text().length() < 8) {
        return false;
    }
    return true;
}
void signupDialog::on_approve_clicked()
{
    if (User::getUserCount() == 0) {
        User u(ui->firstName->text(),
               ui->lastName->text(),
               ui->phone->text(),
               ui->gmail->text(),
               ui->username->text(),
               ui->password->text());
        QFile f("users.txt");
        if (!f.open(QIODevice::Append | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "The file can not be opened!");
            return;
        }
        QTextStream out(&f);
        out << u;
        f.flush();
        f.close();
        return;
    }
    if (!checkGmail() || !checkPassword() || !checkPhone()) {
        QMessageBox::warning(
            this, "Warning", "Your email address or your phone number or your password is invalid!");
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
        QMessageBox::warning(this, "Error", "The file can not be opened!");
        return;
    }
    QTextStream out(&f);
    out << u;
    f.flush();
    f.close();
}
