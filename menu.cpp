#include "menu.h"
#include <QMessageBox>
#include "ui_menu.h"
Menu::Menu(QWidget *parent, const QString &username, QTcpSocket *socket1)
    : QWidget(parent)
    , currentUsername(username)
    , socket{socket1}
{
    connect(socket1, &QTcpSocket::readyRead, this, &Menu::readyRead);
    // connect(socket2, &QTcpSocket::readyRead, this, &Menu::readyRead);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_logout_clicked()
{
    emit logoutRequest();
    this->close();
}

void Menu::on_hsitory_clicked()
{
    if (socket && socket->isOpen()) {
        QString request = "GET_HISTORY:" + currentUsername;
        socket->write(request.toUtf8());
    }
}

void Menu::readyRead()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if (!senderSocket)
        return;

    QByteArray data = senderSocket->readAll();
    QString response = QString::fromUtf8(data);

    if (response.startsWith("HISTORY_DATA:")) {
        response.remove("HISTORY_DATA:");

        QStringList games = response.split("\n", Qt::SkipEmptyParts);
        QString displayText;

        for (const QString &line : games) {
            QStringList fields = line.split(",");
            if (fields.size() == 6) {
                QString opponent = fields[0];
                QString date = fields[1];
                QString r1 = fields[2], r2 = fields[3], r3 = fields[4];
                QString result = fields[5];

                displayText += QString("📅 Date: %1\n🆚 Opponent: %2\n🎮 Rounds: %3 - %4 - %5\n🏁 "
                                       "Result: %6\n\n")
                                   .arg(date, opponent, r1, r2, r3, result);
            }
        }

        QMessageBox::information(this,
                                 " History",
                                 displayText.isEmpty() ? "No match has been played" : displayText);
    }
}
