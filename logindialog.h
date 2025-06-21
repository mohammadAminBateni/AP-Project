#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QWidget>

namespace Ui {
class loginDialog;
}

class loginDialog : public QWidget
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();

private slots:


private:
    Ui::loginDialog *ui;
};

#endif // LOGINDIALOG_H
