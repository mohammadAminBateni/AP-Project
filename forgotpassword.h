#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H

#include <QWidget>

namespace Ui {
class forgotPassword;
}

class forgotPassword : public QWidget
{
    Q_OBJECT

public:
    explicit forgotPassword(QWidget *parent = nullptr);
    ~forgotPassword();
    QString getPhone();

private slots:
    void on_approve_clicked();

private:
    Ui::forgotPassword *ui;
};

#endif // FORGOTPASSWORD_H
