#include "clienthandler.h"
#include "server.h"
#include "user.h"
#include <QDebug>

ClientHandler::ClientHandler(qintptr socketDescriptor, Server *serverInstance, QObject *parent)
    : QObject(parent), server(serverInstance)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);

    qDebug() << "ClientHandler started for" << socket->peerAddress().toString();
}

void ClientHandler::onReadyRead()
{
    while (socket->canReadLine())
    {
        QString message = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << "Received from client:" << message;
        processMessage(message);
    }
}

void ClientHandler::onDisconnected()
{
    qDebug() << "Client disconnected:" << socket->peerAddress().toString();
    socket->deleteLater();
    deleteLater();
}

void ClientHandler::processMessage(const QString &message)
{
    if (message.startsWith("LOGIN "))
    {
        QStringList parts = message.split(" ");
        if (parts.size() != 3)
        {
            socket->write("LOGIN_FAIL Invalid format\n");
            return;
        }

        QString user = parts[1];
        QString pass = parts[2];

        if (server->validateUser(user, pass))
        {
            username = user;
            socket->write("LOGIN_SUCCESS\n");
        }
        else
        {
            socket->write("LOGIN_FAIL Invalid credentials\n");
        }
    }

    else if (message.startsWith("SIGNUP "))
    {
        QStringList parts = message.split(" ");
        if (parts.size() != 7) {
            socket->write("SIGNUP_FAIL Invalid format\n");
            return;
        }

        QString name = parts[1];
        QString family = parts[2];
        QString username = parts[3];
        QString password = parts[4];
        QString phone = parts[5];
        QString email = parts[6];

        if (server->isUsernameTaken(username))
        {
            socket->write("SIGNUP_FAIL Username taken\n");
            return;
        }

        User newUser(name, family, username, password, phone, email);
        server->addUser(newUser);

        socket->write("SIGNUP_SUCCESS\n");
    }

    else if (message == "START_GAME")
    {
        disconnect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
        server->requestGame(socket);
    }

    else
    {
        socket->write("UNKNOWN_COMMAND\n");
    }
}

