#include "server.h"
#include "user.h"
#include <QDebug>
#include <QTcpSocket>
#include "clienthandler.h"
#include "clienthandler.h"
#include "gamesession.h"
#include "QDateTime"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{}

void Server::startServer(quint16 port)
{
    loadUsers();

    if (this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Failed to start server:" << this->errorString();
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    ClientHandler *handler = new ClientHandler(socketDescriptor, this, this);
}

void Server::loadUsers()
{
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() == 6)
        {
            User user(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
            users[user.username] = user;
        }
    }

    file.close();
}

void Server::saveUser( User &user)
{
    User u;
    QFile file("users.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << user.name << "|" << user.family << "|" << user.username << "|"
        << user.password<< "|" << user.phone << "|" << user.email << "\n";

    file.close();
}

bool Server::isUsernameTaken(const QString& username)
{
    return users.contains(username);
}

void Server::addUser(User& user)
{
    users[user.username] = user;
    saveUser(user);
}
bool Server::validateUser(const QString& username, const QString& password1)
{
    if (!users.contains(username))
        return false;

    return users[username].password == password1;
}
void Server::requestGame(QTcpSocket* player)
{
    waitingPlayers.append(player);
    qDebug() << "Player requested game. Waiting list size:" << waitingPlayers.size();

    if (waitingPlayers.size() >= 2)
    {
        QTcpSocket* p1 = waitingPlayers.takeFirst();
        QTcpSocket* p2 = waitingPlayers.takeFirst();

        GameSession* session = new GameSession(p1, p2, this, this);
        session->start();
    }
}
    void Server::saveGameHistory(const QString& player1, const QString& player2, const QString& winner, int score1, int score2) {
    QFile file("game_history.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << player1 << "," << player2 << "," << winner << ","
        << score1 << "," << score2 << "," << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n";

    file.close();
}

    void Server::loadGameHistory()
{
    QFile file("game_history.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << "Game history: " << line;
    }

    file.close();
}
