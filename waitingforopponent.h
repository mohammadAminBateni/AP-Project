#ifndef WAITINGFOROPPONENT_H
#define WAITINGFOROPPONENT_H

#include <QWidget>

namespace Ui {
class waitingforopponent;
}

class waitingforopponent : public QWidget
{
    Q_OBJECT

public:
    explicit waitingforopponent(QWidget *parent = nullptr);
    ~waitingforopponent();

private:
    Ui::waitingforopponent *ui;
};

#endif // WAITINGFOROPPONENT_H
