#include "gamesession.h"
#include "server.h"
#include <QDebug>

GameSession::GameSession(QTcpSocket* player1, QTcpSocket* player2,Server* server, QObject* parent)
    : QObject(parent), p1(player1), p2(player2)
{

}
void GameSession::start()
{
    dealCards();
    sendToBoth("GAME_START\n");

    QString h1, h2;
    for (const Card& c : hand1)
        h1 += QString::number(c.value) + "-" + c.suit + " ";
    for (const Card& c : hand2)
        h2 += QString::number(c.value) + "-" + c.suit + " ";

    p1->write(("HAND " + h1 + "\n").toUtf8());
    p2->write(("HAND " + h2 + "\n").toUtf8());

    connect(p1, &QTcpSocket::readyRead, this, &GameSession::handleInput);
    connect(p2, &QTcpSocket::readyRead, this, &GameSession::handleInput);

    connect(p1, &QTcpSocket::disconnected, this, [=]()
            {
        p2->write("OPPONENT_DISCONNECTED\n");
        this->deleteLater();
    });

    connect(p2, &QTcpSocket::disconnected, this, [=]()
            {
        p1->write("OPPONENT_DISCONNECTED\n");
        this->deleteLater();
    });

    p1->write("SELECT_CARD\n");
    p2->write("SELECT_CARD\n");
}
void GameSession::sendToBoth(const QString& message)
{
    if (p1) p1->write(message.toUtf8());
    if (p2) p2->write(message.toUtf8());
}
void GameSession::dealCards()
{
    QStringList suits = {"Diamond", "Gold", "Dollar", "Coin"};
    for (const QString& suit : suits)
    {
        for (int val = 2; val <= 14; ++val)
        {
            deck.append(Card{val, suit});
        }
    }

    std::random_shuffle(deck.begin(), deck.end());

    for (int i = 0; i < 7; ++i)
        hand1.append(drawCard());

    for (int i = 0; i < 7; ++i)
        hand2.append(drawCard());
}
Card GameSession::drawCard()
{
    Card c = deck.last();
    deck.removeLast();
    return c;
}
void GameSession::handleInput()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());

    while (sock->canReadLine())
    {
        QString msg = QString::fromUtf8(sock->readLine()).trimmed();

        if (msg.startsWith("SELECT ")) {
            bool ok;
            int index = msg.section(' ', 1).toInt(&ok);

            if (!ok || index < 0 || index >= 7)
            {
                sock->write("INVALID_SELECTION\n");
                continue;
            }

            if (sock == p1 && !selected1Ready)
            {
                selected1 = hand1[index];
                hand1.removeAt(index);
                selected1Ready = true;
                sock->write("CARD_SELECTED\n");
            } else if (sock == p2 && !selected2Ready)
            {
                selected2 = hand2[index];
                hand2.removeAt(index);
                selected2Ready = true;
                sock->write("CARD_SELECTED\n");
            }

            if (selected1Ready && selected2Ready)
            {
                processRound();
            }
        }
    }
}
void GameSession::processRound()
{
    QString msg1 = QString("OPPONENT_CARD %1-%2\n").arg(selected2.value).arg(selected2.suit);
    QString msg2 = QString("OPPONENT_CARD %1-%2\n").arg(selected1.value).arg(selected1.suit);
    p1->write(msg1.toUtf8());
    p2->write(msg2.toUtf8());

    if (selected1.value > selected2.value)
    {
        score1++;
        sendToBoth("ROUND_RESULT Player1_Wins\n");
    } else if (selected2.value > selected1.value)
    {
        score2++;
        sendToBoth("ROUND_RESULT Player2_Wins\n");
    } else {
        sendToBoth("ROUND_RESULT Tie\n");
    }

    selected1Ready = selected2Ready = false;
    round++;

    if (round >= 3)
    {
        QString winner = (score1 > score2) ? "Player1" : (score2 > score1) ? "Player2" : "Tie";
        server->saveGameHistory("Player1", "Player2", winner, score1, score2);

        sendToBoth("GAME_OVER " + winner + "\n");

        disconnect(p1, nullptr, this, nullptr);
        disconnect(p2, nullptr, this, nullptr);
        this->deleteLater();
    }
    else
    {
        p1->write("SELECT_CARD\n");
        p2->write("SELECT_CARD\n");
    }
}
