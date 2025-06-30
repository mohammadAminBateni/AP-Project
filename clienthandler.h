#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class Server;

class ClientHandler : public QObject
{
    Q_OBJECT

public:
    explicit ClientHandler(qintptr socketDescriptor, Server *server, QObject *parent = nullptr);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *socket;
    Server *server;
    QString username;

    void processMessage(const QString &message);
};

#endif // CLIENTHANDLER_H
