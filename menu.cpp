#include "menu.h"
#include <QMessageBox>
#include "editprofiledialog.h"
#include "gamepage.h"
#include "ui_menu.h"
Menu::Menu(QWidget *parent, const QString &username, QTcpSocket *socket1)
    : QWidget(parent)
    , ui(new Ui::Menu)
    , currentUsername(username)
    , socket{socket1}
{
    ui->setupUi(this);
    connect(socket1, &QTcpSocket::readyRead, this, &Menu::readyRead);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_logout_clicked()
{
    if (socket && socket->isOpen()) {
        socket->write("LOGOUT:" + currentUsername.toUtf8());
        socket->disconnectFromHost();
    }
    QApplication::quit();
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

void Menu::on_start_clicked()
{
    QString message = "START_GAME";
    socket->write(message.toUtf8() + "\n");
    GamePage *g = new GamePage(nullptr, currentUsername, socket);
    g->show();
    this->close();
}

void Menu::on_edit_clicked()
{
    EditProfileDialog *e = new EditProfileDialog(nullptr, socket, currentUsername);
    e->show();
    this->close();
}
