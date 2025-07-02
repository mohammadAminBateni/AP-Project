#include "clienthandler.h"
#include "server.h"
#include "user.h"
#include <QDebug>
#include <QCryptographicHash>

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
    if (message.startsWith("LOGIN:"))
    {
        QStringList parts = message.split(":");
        if (parts.size() != 3)
        {
            socket->write("LOGIN_FAIL Invalid format\n");
            return;
        }

        QString user = parts[1];
        QString hashedPass = parts[2];

        if (server->validateUser(user, hashedPass))
        {
            username = user;
            qDebug() << "Login\n";
            socket->write("LOGIN_SUCCESS\n");
        }
        else
        {
            socket->write("LOGIN_FAIL Invalid credentials\n");
        }
    }

    else if (message.startsWith("SIGNUP:"))
    {
        QStringList parts = message.split(":");
        if (parts.size() != 7)
        {
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

        QString hashedPassword = QString(
            QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

        User newUser(name, family, username, hashedPassword, phone, email);
        server->addUser(newUser);
        qDebug() << "Signup\n";
        socket->write("SIGNUP_SUCCESS\n");
    }

    else if (message == "START_GAME")
    {
        qDebug() << "Start/n";
        disconnect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
        server->requestGame(socket);
    }
    else if (message.startsWith("UPDATE_PROFILE:"))
    {
        QStringList parts = message.split(":");
        if (parts.size() != 8)
        {
            socket->write("UPDATE_FAIL:Invalid format\n");
            return;
        }

        QString oldUsername = parts[1].trimmed();
        QString newFirst = parts[2].trimmed();
        QString newLast = parts[3].trimmed();
        QString newPhone = parts[4].trimmed();
        QString newEmail = parts[5].trimmed();
        QString newUsername = parts[6].trimmed();
        QString newPassword = parts[7].trimmed();

        QFile file("users.txt");
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            socket->write("UPDATE_FAIL:Cannot open user file\n");
            return;
        }

        QTextStream stream(&file);
        QStringList allUsers;
        bool found = false;
        bool usernameTaken = false;

        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList fields = line.split("|");

            if (fields.size() < 6)
            {
                allUsers << line;
                continue;
            }

            QString existingUsername = fields[4].trimmed();

            if (existingUsername == oldUsername)
            {
                fields[0] = newFirst;
                fields[1] = newLast;
                fields[2] = newPhone;
                fields[3] = newEmail;
                fields[4] = newUsername;
                fields[5] = newPassword;
                found = true;
            }
            else if (existingUsername == newUsername && oldUsername != newUsername)
            {
                usernameTaken = true;
            }

            allUsers << fields.join("|");
        }

        file.resize(0);
        QTextStream out(&file);
        for (const QString& line : allUsers)
        {
            out << line << "\n";
        }
        file.close();

        if (!found) {
            socket->write("UPDATE_FAIL:User not found\n");
        }
        else if (usernameTaken) {
            socket->write("UPDATE_FAIL:Username already taken\n");
        }
        else {
            socket->write("UPDATE_SUCCESS\n");
        }
    }
    else
    {
        socket->write("UNKNOWN_COMMAND\n");
    }
}
