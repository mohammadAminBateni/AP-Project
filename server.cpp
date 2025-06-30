#include "server.h"
#include <QDebug>
#include <QTcpSocket>
#include "clienthandler.h"
#include "clienthandler.h"

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
        QStringList parts = line.split(',');
        if (parts.size() == 6) {
            User user(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
            users[user.username] = user;
        }
    }

    file.close();
}

void Server::saveUser(const User& user)
{
    QFile file("users.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << user.name << "," << user.family << "," << user.username << ","
        << user.password << "," << user.phone << "," << user.email << "\n";

    file.close();
}

bool Server::isUsernameTaken(const QString& username)
{
    return users.contains(username);
}

void Server::addUser(const User& user)
{
    users[user.username] = user;
    saveUser(user);
}
bool Server::validateUser(const QString& username, const QString& password)
{
    if (!users.contains(username))
        return false;

    return users[username].password == password;
}
