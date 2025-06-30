#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QObject>
#include <QString>
#include "user.h"
#include <QMap>
#include <QFile>
#include <QTextStream>

class Server : public QTcpServer
{
    Q_OBJECT

public:

    void loadUsers();
    void saveUser(const User& user);
    bool isUsernameTaken(const QString& username);
    void addUser(const User& user);
    explicit Server(QObject *parent = nullptr);
    void startServer(quint16 port = 1234);
    bool validateUser(const QString& username, const QString& password);

protected:

    void incomingConnection(qintptr socketDescriptor) override;

private:
     QMap<QString, User> users;
    QList<QTcpSocket*> clients;
};

#endif // SERVER_H
