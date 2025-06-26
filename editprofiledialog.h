#ifndef EDITPROFILEDIALOG_H
#define EDITPROFILEDIALOG_H

#include <QWidget>

namespace Ui {
class EditProfileDialog;
}

class EditProfileDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EditProfileDialog(QWidget *parent = nullptr);
    ~EditProfileDialog();

private:
    Ui::EditProfileDialog *ui;
};

#endif // EDITPROFILEDIALOG_H
