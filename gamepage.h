#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QLabel>
#include <QTcpSocket>
#include <QWidget>
#include "qpushbutton.h"
namespace Ui {
class GamePage;
}

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr,
                      const QString &username = "",
                      QTcpSocket *socket1 = nullptr,
                      QTcpSocket *socket2 = nullptr);
    ~GamePage();

private slots:
    void onCardClicked();
    void displayCards(const QStringList &cardList);

private:
    Ui::GamePage *ui;
    QVector<QPushButton *> cards;
    QTcpSocket *sockets[2];
    QString username;
    QVector<QLabel *> labels;
    int currentSelection = 0;
};

#endif // GAMEPAGE_H
