#include "editprofiledialog.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "ui_editprofiledialog.h"
#include "user.h"
EditProfileDialog::EditProfileDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditProfileDialog)
{
    ui->setupUi(this);
}

EditProfileDialog::~EditProfileDialog()
{
    delete ui;
}

void EditProfileDialog::on_approve_clicked()
{
    // 1. باز کردن فایل به صورت خواندن و نوشتن
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open users file!");
        return;
    }

    // 2. خواندن تمام کاربران
    QTextStream stream(&file);
    QStringList allUsers;
    User currentUser;
    bool userFound = false;

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        User u = User::fromString(
            line); // فرض می‌کنیم متدی برای تبدیل رشته به User داریم

        if (u.getUsername() == ui->currentUsername->text()) {
            // 3. به‌روزرسانی فیلدها
            if (!ui->firstName->text().isEmpty())
                u.setFirstName(ui->firstName->text());
            if (!ui->lastName->text().isEmpty())
                u.setLastName(ui->lastName->text());
            if (!ui->phone->text().isEmpty())
                u.setPhone(ui->phone->text());
            if (!ui->gmail->text().isEmpty())
                u.setGmail(ui->gmail->text());
            if (!ui->password->text().isEmpty())
                u.setPassword(ui->password->text());

            // 4. بررسی نام کاربری جدید
            if (!ui->username->text().isEmpty()) {
                if (ui->username->text() != ui->currentUsername->text()) {
                    // بررسی تکراری نبودن نام کاربری جدید
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
                        return;
                    }
                    u.setUsername(ui->username->text());
                }
            }

            userFound = true;
        }
        allUsers << u.toString(); // ذخیره تمام کاربران (با تغییرات)
    }

    // 5. اگر کاربر پیدا نشد
    if (!userFound) {
        QMessageBox::warning(this, "Error", "User not found!");
        file.close();
        return;
    }

    // 6. بازنویسی کامل فایل
    file.resize(0); // پاک کردن محتوای قدیمی
    for (const QString &userLine : allUsers) {
        stream << userLine << "\n";
    }

    file.close();
    QMessageBox::information(this, "Success", "Profile updated successfully!");
}
