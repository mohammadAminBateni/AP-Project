#include "gamepage.h"
#include <QMessageBox>
#include <QTimer>
#include "gameresult.h"
#include "mainwindow.h"
#include "menu.h"
#include "swapdialog.h"
#include "ui_gamepage.h"
#include "user.h"
GamePage::GamePage(QWidget *parent, const QString &userName, QTcpSocket *socket1)
    : QWidget(parent)
    , ui(new Ui::GamePage)
    , socket{socket1}
    , username(userName)
{
    ui->setupUi(this);
    cards.push_back(ui->card1);
    cards.push_back(ui->card2);
    cards.push_back(ui->card3);
    cards.push_back(ui->card4);
    cards.push_back(ui->card5);
    cards.push_back(ui->card6);
    cards.push_back(ui->card7);
    for (auto x : cards) {
        connect(x, &QPushButton::clicked, this, &GamePage::onCardClicked);
    }
    labels.push_back(ui->crd1);
    labels.push_back(ui->crd2);
    labels.push_back(ui->crd3);
    labels.push_back(ui->crd4);
    labels.push_back(ui->crd5);
    connect(socket, &QTcpSocket::readyRead, this, &GamePage::readyRead);
}

GamePage::~GamePage()
{
    delete ui;
}
void GamePage::displayCards(const QStringList &cardList)
{
    for (int i = 0; i < cardList.size() && i < cards.size(); ++i) {
        QString card = cardList[i];
        QString imagePath = ":new/prefix1/cards/" + card + ".JPG";

        cards[i]->setIcon(QIcon(imagePath));
        cards[i]->setIconSize(QSize(100, 150));
        cards[i]->setEnabled(true);
        cards[i]->setProperty("cardName", card);
    }
}
void GamePage::onCardClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;
    if (sd && sd->isVisible()) {
        QString card = btn->property("cardName").toString();
        sd->setSelectedCard(card);
        sd->accept();
        return;
    }
    QString card = btn->property("cardName").toString();

    QString message = "CHOOSE_CARD:" + card;
    socket->write(message.toUtf8());
    QString imagePath = ":new/prefix1/cards/" + card + ".JPG";
    QPixmap cardImg(imagePath);
    labels[currentSelection]->setPixmap(cardImg.scaled(100, 150, Qt::KeepAspectRatio));
    btn->setEnabled(false);
    currentSelection++;
    if (currentSelection == 5) {
        for (QPushButton *b : cards)
            b->setEnabled(false);
    }
}

void GamePage::on_exit_clicked()
{
    QString msg = "EXIT_GAME:" + username;
    socket->write(msg.toUtf8());

    this->close();
    Menu *m = new Menu(parentWidget(), username, socket);
    m->show();
}

void GamePage::on_stopOrContinue_clicked()
{
    if (isGamePaused) {
        if (u.getPauseRequests() >= 2) {
            ui->stopOrContinue->setEnabled(false);
        } else {
            u.setPauseRequests(u.getPauseRequests() + 1);
            timer = new QTimer;
            elapsedTime = QTime(0, 0, 0);
            connect(this, &GamePage::updateTimerSignal, this, &GamePage::updateTimer);
            connect(timer, &QTimer::timeout, this, [this]() {
                elapsedTime = elapsedTime.addSecs(1);
                emit updateTimer(elapsedTime.toString("hh:mm:ss"));
                if (elapsedTime.second() >= 20) {
                    timer->stop();
                    QString msg = "PAUSE_TIMEOUT:" + u.getUsername();
                    socket->write(msg.toUtf8());
                    this->close();
                }
            });
            timer->start(1000);
            QString msg = "PAUSE_REQUEST:" + u.getUsername();
            socket->write(msg.toUtf8());
            ui->stopOrContinue->setText("Resume");
        }
    } else {
        isGamePaused = false;
        if (timer)
            timer->stop();
        QString msg = "RESUME_REQUEST:" + u.getUsername();
        socket->write(msg.toUtf8());
        ui->stopOrContinue->setText("Stop");
    }
}
void GamePage::updateTimer(QString time)
{
    ui->timer->display(time);
}

void GamePage::on_swap_clicked()
{
    QString req = "ROUND_NUMBER:";
    socket->write(req.toUtf8());

    if (socket->waitForReadyRead(3000)) {
        QByteArray data1 = socket->readAll();
        QString serverResponse = QString::fromUtf8(data1);

        if (serverResponse.startsWith("ROUND_NUMBER:")) {
            QString number = serverResponse.section(':', 1);
            if (number.toInt() != 5) {
                sd = new SwapDialog(this);
                sd->exec();
                QString selectedCard = sd->getSelectedCard();
                delete sd;
                sd = nullptr;
                if (!selectedCard.isEmpty()) {
                    QString message = "SWAP_REQUEST:" + selectedCard;
                    socket->write(message.toUtf8());
                } else {
                    QMessageBox::warning(this, "Swap", "No card was selected for swap.");
                }

            } else {
                QMessageBox::warning(this,
                                     "Error",
                                     "It is the 5th round and you cannot swap cards.");
            }
        }
    }
}

void GamePage::readyRead()
{
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data);

    if (response.startsWith("RESULT:")) {
        QString gameResult = response.section(':', 1);
        GameResult *resultPage = new GameResult(nullptr, gameResult);
        resultPage->show();
        this->close();
    }
}
