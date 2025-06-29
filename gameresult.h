#ifndef GAMERESULT_H
#define GAMERESULT_H

#include <QWidget>

namespace Ui {
class GameResult;
}

class GameResult : public QWidget
{
    Q_OBJECT

public:
    explicit GameResult(QWidget *parent = nullptr, QString gResult = "lose");
    ~GameResult();

private slots:
    void on_return_2_clicked();

private:
    Ui::GameResult *ui;
    QString result;
};

#endif // GAMERESULT_H
