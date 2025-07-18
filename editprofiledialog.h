#ifndef EDITPROFILEDIALOG_H
#define EDITPROFILEDIALOG_H

#include <QWidget>
#include "qtcpsocket.h"
#include "user.h"

namespace Ui {
class EditProfileDialog;
}

class EditProfileDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EditProfileDialog(QWidget *parent = nullptr, QTcpSocket *socket = nullptr, const QString &currentUsername = "");
    ~EditProfileDialog();
    void setCurrentUser(const User &user);

private slots:
    void on_approve_clicked();
    void readyRead();

private:
    Ui::EditProfileDialog *ui;
    QTcpSocket *socket;
    User updatedUser;
    QString currentUsername;
};

#endif // EDITPROFILEDIALOG_H
