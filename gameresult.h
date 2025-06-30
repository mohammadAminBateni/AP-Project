#ifndef GAMERESULT_H
#define GAMERESULT_H

#include <QWidget>
#include "qtcpsocket.h"

namespace Ui {
class GameResult;
}

class GameResult : public QWidget
{
    Q_OBJECT

public:
    explicit GameResult(QWidget *parent = nullptr,
                        QString gResult = "lose",
                        QTcpSocket *sock = nullptr,
                        QString u = "");
    ~GameResult();

private slots:
    void on_return_2_clicked();

private:
    Ui::GameResult *ui;
    QString result;
    QString username;
    QTcpSocket *socket;
};

#endif // GAMERESULT_H
