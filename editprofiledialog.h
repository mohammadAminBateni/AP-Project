#ifndef EDITPROFILEDIALOG_H
#define EDITPROFILEDIALOG_H

#include <QWidget>
#include "qtcpsocket.h"

namespace Ui {
class EditProfileDialog;
}

class EditProfileDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EditProfileDialog(QWidget *parent = nullptr, QTcpSocket *socket = nullptr);
    ~EditProfileDialog();

private slots:
    void on_approve_clicked();

private:
    Ui::EditProfileDialog *ui;
    QTcpSocket *socket;
};

#endif // EDITPROFILEDIALOG_H
