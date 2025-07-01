#include "gamepage.h"
#include <QMessageBox>
#include <QTimer>
#include "gameresult.h"
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
    connect(socket, &QTcpSocket::disconnected, this, &GamePage::handleDisconnection);
}

GamePage::~GamePage()
{
    delete ui;
}

void GamePage::handleNotChoosingCard()
{
    if (inactivity && inactivity->isActive())
        return;

    inactivity = new QTimer(this);
    countDown = new QTimer(this);
    inactivityElapsed = QTime(0, 0, 0);
    countdownElapsed = QTime(0, 0, 10);

    connect(inactivity, &QTimer::timeout, this, [=]() {
        inactivityElapsed = inactivityElapsed.addSecs(1);
        ui->timer->display(inactivityElapsed.toString("mm:ss"));

        if (inactivityElapsed == QTime(0, 0, 20)) {
            inactivity->stop();
            QMessageBox::warning(this, "Time Warning", "You have 10 seconds to pick a card!");

            connect(countDown, &QTimer::timeout, this, [=]() {
                countdownElapsed = countdownElapsed.addSecs(-1);
                ui->timer->display(countdownElapsed.toString("mm:ss"));

                if (countdownElapsed == QTime(0, 0, 0)) {
                    countDown->stop();

                    QString selectedCard;
                    for (QPushButton *btn : cards) {
                        if (btn->isEnabled()) {
                            selectedCard = btn->property("cardName").toString();
                            break;
                        }
                    }

                    if (!selectedCard.isEmpty()) {
                        QString msg = "CHOOSE_CARD:" + selectedCard;
                        socket->write(msg.toUtf8());
                        QPixmap cardImg(":/new/prefix1/cards/" + selectedCard + ".JPG");
                        labels[currentSelection]->setPixmap(
                            cardImg.scaled(100, 150, Qt::KeepAspectRatio));
                        currentSelection++;
                        for (QPushButton *btn : cards)
                            btn->setEnabled(false);
                    }

                    notChoosingCard++;
                    if (notChoosingCard >= 2) {
                        QString msg = "FORFEIT:" + username;
                        socket->write(msg.toUtf8());
                        this->close();
                    }
                }
            });

            countDown->start(1000);
        }
    });

    inactivity->start(1000);
}

void GamePage::displayCards(const QStringList &cardList)
{
    for (int i = 0; i < cardList.size() && i < cards.size(); ++i) {
        QString card = cardList[i];
        QString imagePath = ":/new/prefix1/cards/" + card + ".JPG";

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
    QString imagePath = ":/new/prefix1/cards/" + card + ".JPG";
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
            timer1 = new QTimer(this);
            elapsedTime = QTime(0, 0, 0);
            connect(this, &GamePage::updateTimerSignal, this, &GamePage::updateTimer);
            connect(timer1, &QTimer::timeout, this, [this]() {
                elapsedTime = elapsedTime.addSecs(1);
                emit updateTimer(elapsedTime.toString("hh:mm:ss"));
                if (elapsedTime.second() >= 20) {
                    timer1->stop();
                    QString msg = "PAUSE_TIMEOUT:" + username;
                    socket->write(msg.toUtf8());
                    this->close();
                }
            });
            timer1->start(1000);
            QString msg = "PAUSE_REQUEST:" + username;
            socket->write(msg.toUtf8());
            ui->stopOrContinue->setText("Resume");
        }
    } else {
        isGamePaused = false;
        if (timer1)
            timer1->stop();
        QString msg = "RESUME_REQUEST:" + username;
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
}

void GamePage::handleDisconnection()
{
    timer2 = new QTimer(this);
    elapsedTime = QTime(0, 1, 0);
    connect(this, &GamePage::updateTimerSignal, this, &GamePage::updateTimer);
    connect(timer2, &QTimer::timeout, this, [=]() {
        elapsedTime = elapsedTime.addSecs(-1);
        emit updateTimer(elapsedTime.toString("hh:mm:ss"));
        if (elapsedTime == QTime(0, 0, 0)) {
            timer2->stop();
            QString msg = "DISCONNECTED:" + username;
            socket->write(msg.toUtf8());
            this->close();
            Menu *m = new Menu(nullptr, username, socket);
            m->show();
        }
    });
    timer2->start(1000);
}

void GamePage::readyRead()
{
    QString response = QString::fromUtf8(socket->readAll());

    if (response.startsWith("RESULT:")) {
        QString gameResult = response.section(':', 1);
        GameResult *resultPage = new GameResult(nullptr, gameResult, socket, username);
        resultPage->show();
        this->close();
        this->deleteLater();
        return;
    } else if (response.startsWith("ROUND_NUMBER:")) {
        QString number = response.section(':', 1);
        if (number.toInt() != 5) {
            sd = new SwapDialog(this, socket);
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
            QMessageBox::warning(this, "Error", "It is the 5th round and you cannot swap cards.");
        }
    } else if (response == "YOUR_TURN") {
        handleNotChoosingCard();
    } else if (response.startsWith("SWAP_RESPONSE:")) {
        QString res = response.section(':', 1);
        if (res == "NO") {
            QMessageBox::information(this, "Info", "Opponent rejected the card swap.");
        }
    } else if (response.startsWith("STARTER:")) {
        QString starterUsername = response.section(':', 1);
        if (starterUsername == username) {
            QMessageBox::information(this, "Round Info", "🎮 You start this round.");
            isMyTurn = true;
        } else {
            QMessageBox::information(this, "Round Info", "⏳ Opponent starts this round.");
            isMyTurn = false;
        }
    } else if (response.startsWith("ROUND:")) {
        int roundNum = response.section(':', 1).toInt();
        currentRound = roundNum;
        ui->roundNumber->setText("Round: " + QString::number(currentRound));
    }
}
