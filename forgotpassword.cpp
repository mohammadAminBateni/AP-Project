#include "forgotpassword.h"
#include <QFile>
#include "ui_forgotpassword.h"
#include "user.h"
#include<QMessageBox>
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
    QFile f("users.txt");
    QTextStream in(&f);
    while (!f.atEnd()) {
        User u;
        in >> u;
        if (u.getPhone() == ui->phone->text()) {
            u.setPassword(ui->newPassword->text());
            f.close();
            return;
        }
    }
    QMessageBox::warning(this, "Warning", "This phone number does not exist!");
}
