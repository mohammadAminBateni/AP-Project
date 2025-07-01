#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QVector>
#include <QVector>
#include <QMap>
class Server;

struct Card
{
    int value;
    QString suit;
};

class GameSession : public QObject
{
    Q_OBJECT

public:
    GameSession(QTcpSocket* player1, QTcpSocket* player2,Server* server, QObject* parent = nullptr);
    void start();

private:
    Server* server;
    QTcpSocket* p1;
    QTcpSocket* p2;

    QVector<Card> deck;
    QVector<Card> hand1;
    QVector<Card> hand2;

    int round = 0;
    int score1 = 0;
    int score2 = 0;

    void sendToBoth(const QString& message);
    void dealCards();
    Card drawCard();
    void handleInput();
    void processRound();

    Card selected1;
    Card selected2;
    bool selected1Ready = false;
    bool selected2Ready = false;
};

#endif // GAMESESSION_H
