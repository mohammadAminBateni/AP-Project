#include "waitingforopponent.h"
#include "gamepage.h"
#include "ui_waitingforopponent.h"

waitingforopponent::waitingforopponent(QWidget *parent, QString u, QTcpSocket *sock)
    : QWidget(parent)
    , ui(new Ui::waitingforopponent)
    , socket(sock)
    , username(u)
{
    ui->setupUi(this);
    connect(socket, &QTcpSocket::readyRead, this, &waitingforopponent::onReadyRead);
}

waitingforopponent::~waitingforopponent()
{
    delete ui;
}
void waitingforopponent::onReadyRead()
{
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        if (line == "GAME_START") {
            GamePage *game = new GamePage(nullptr, username, socket);
            game->show();
            this->close();
            break;
        }
    }
}
