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
                        socket->write(msg.toUtf8() + "\n");
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
                        socket->write(msg.toUtf8() + "\n");
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
        QString cardName = card.section(':', 0, 0);
        QString imagePath = ":/new/prefix1/" + card.section(':', 1);

        cards[i]->setIcon(QIcon(imagePath));
        cards[i]->setIconSize(QSize(100, 150));
        cards[i]->setProperty("cardName", cardName);

        cards[i]->setEnabled(isMyTurn);
    }
}

void GamePage::onCardClicked()
{
    if (!isMyTurn)
        return; // اگه نوبتت نیست، کاری نکن

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
    socket->write(message.toUtf8() + "\n");

    QString value = card.section('-', 0, 0);
    QString suit = card.section('-', 1);
    QString imagePath = ":/new/prefix1/cards/" + suit + "-" + value + ".JPG";

    QPixmap cardImg(imagePath);
    labels[currentSelection]->setPixmap(cardImg.scaled(100, 150, Qt::KeepAspectRatio));
    btn->setEnabled(false);
    currentSelection++;

    if (currentSelection == 5) {
        for (QPushButton *b : cards)
            b->setEnabled(false);
    }

    isMyTurn = false; // نوبتت تموم شد
}

void GamePage::on_exit_clicked()
{
    QString msg = "EXIT_GAME:" + username;
    socket->write(msg.toUtf8() + "\n");

    this->close();
    Menu *m = new Menu(parentWidget(), username, socket);
    m->show();
}

void GamePage::on_stopOrContinue_clicked()
{
    if (!socket || !socket->isOpen()) {
        QMessageBox::warning(this, "Error", "Connection lost.");
        return;
    }

    if (!isGamePaused) {
        if (u.getPauseRequests() >= 2) {
            ui->stopOrContinue->setEnabled(false);
            return;
        }
        u.setPauseRequests(u.getPauseRequests() + 1);

        timer1 = new QTimer(this);
        elapsedTime = QTime(0, 0, 0);
        connect(this, &GamePage::updateTimerSignal, this, &GamePage::updateTimer);
        connect(timer1, &QTimer::timeout, this, [this]() {
            elapsedTime = elapsedTime.addSecs(1);
            emit updateTimer(elapsedTime.toString("hh:mm:ss"));
            if (elapsedTime.second() >= 20) {
                timer1->stop();
                if (socket && socket->isOpen()) {
                    QString msg = "PAUSE_TIMEOUT:" + username;
                    socket->write(msg.toUtf8() + "\n");
                }
                this->close();
            }
        });
        timer1->start(1000);

        QString msg = "PAUSE_REQUEST:" + username;
        socket->write(msg.toUtf8() + "\n");

        ui->stopOrContinue->setText("Resume");
        isGamePaused = true;
    } else {
        if (timer1)
            timer1->stop();

        QString msg = "RESUME_REQUEST:" + username;
        socket->write(msg.toUtf8() + "\n");

        ui->stopOrContinue->setText("Stop");
        isGamePaused = false;
    }
}

void GamePage::updateTimer(QString time)
{
    ui->timer->display(time);
}

void GamePage::on_swap_clicked()
{
    QString req = "ROUND_NUMBER:";
    socket->write(req.toUtf8() + "\n");
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
            socket->write(msg.toUtf8() + "\n");
            this->close();
            Menu *m = new Menu(nullptr, username, socket);
            m->show();
        }
    });
    timer2->start(1000);
}

void GamePage::readyRead()
{
    QString response = QString::fromUtf8(socket->readAll()).trimmed();

    QStringList messages = response.split("\n", Qt::SkipEmptyParts);
    for (const QString &msg : messages) {
        if (msg == "GAME_START") {
            startChoosingCard();
        } else if (msg.startsWith("HAND ")) {
            QStringList cardList = msg.mid(5).split(" ", Qt::SkipEmptyParts);
            displayCards(cardList);
        } else if (msg == "YOUR_TURN") {
            isMyTurn = true;
            for (QPushButton *b : cards) {
                if (b->property("cardName").toString()
                    != "") // یا هر شرطی که کارت‌ها فعال بشن
                    b->setEnabled(true);
            }
            handleNotChoosingCard();
        } else if (msg.startsWith("OPPONENT_CARD")) {
            QString oppCard = msg.section(' ', 1);
            QMessageBox::information(this, "Opponent Card", "Opponent played: " + oppCard);
        } else if (msg.startsWith("ROUND_RESULT")) {
            QString result = msg.section(' ', 1);
            QMessageBox::information(this, "Round Result", result);
        } else if (msg.startsWith("ROUND:")) {
            int roundNum = msg.section(':', 1).toInt();
            currentRound = roundNum;
            ui->roundNumber->setText("Round: " + QString::number(currentRound));
        } else if (msg.startsWith("STARTER:")) {
            QString starterUsername = msg.section(':', 1);
            if (starterUsername == username) {
                isMyTurn = true;
                QMessageBox::information(this, "Starter", "🎮 You start this round.");
            } else {
                isMyTurn = false;
                QMessageBox::information(this, "Starter", "⏳ Opponent starts this round.");
            }
        } else if (msg == "YOUR_TURN") {
            handleNotChoosingCard();
        } else if (msg.startsWith("SWAP_RESPONSE:")) {
            QString res = msg.section(':', 1);
            if (res == "NO") {
                QMessageBox::information(this, "Swap", "Opponent rejected the card swap.");
            } else if (res == "YES") {
                QMessageBox::information(this, "Swap", "Swap accepted.");
            }
        } else if (msg.startsWith("ROUND_NUMBER:")) {
            QString number = msg.section(':', 1);
            if (number.toInt() != 5) {
                sd = new SwapDialog(this, socket);
                sd->exec();
                QString selectedCard = sd->getSelectedCard();
                delete sd;
                sd = nullptr;

                if (!selectedCard.isEmpty()) {
                    QString message = "SWAP_REQUEST:" + selectedCard + "\n";
                    socket->write(message.toUtf8());
                } else {
                    QMessageBox::warning(this, "Swap", "No card was selected for swap.");
                }
            } else {
                QMessageBox::warning(this,
                                     "Swap Error",
                                     "It is the 5th round. You cannot swap cards.");
            }
        } else if (msg.startsWith("SWAP_REQUEST:")) {
            QString offeredCard = msg.section(':', 1);
            sd = new SwapDialog(this, socket);
            QMessageBox::information(this,
                                     "Swap Request",
                                     "Opponent wants to swap card: " + offeredCard);

            sd->exec();
            QString selectedCard = sd->getSelectedCard();
            delete sd;
            sd = nullptr;

            if (!selectedCard.isEmpty()) {
                socket->write("SWAP_RESPONSE:YES\n");
            } else {
                socket->write("SWAP_RESPONSE:NO\n");
            }
        } else if (msg == "PAUSE_TIMEOUT") {
            QMessageBox::warning(this, "Pause", "Time expired during pause. Game forfeited.");
            this->close();
            Menu *m = new Menu(nullptr, username, socket);
            m->show();
        } else if (msg.startsWith("GAME_OVER")) {
            QString winner = msg.section(' ', 1);
            GameResult *resultPage = new GameResult(nullptr, winner, socket, username);
            resultPage->show();
            this->close();
        } else if (msg.startsWith("DISCONNECTED:")) {
            QString disconnectedUser = msg.section(':', 1);
            QMessageBox::warning(this, "Disconnected", disconnectedUser + " disconnected.");
            this->close();
            Menu *m = new Menu(nullptr, username, socket);
            m->show();
        } else if (msg == "OPPONENT_DISCONNECTED") {
            QMessageBox::warning(this, "Disconnected", "Opponent disconnected.");
            this->close();
            Menu *m = new Menu(nullptr, username, socket);
            m->show();
        } else if (msg == "GAME_PAUSED") {
            QMessageBox::information(this, "Game Paused", "The game has been paused by opponent.");
            isGamePaused = true;
            ui->stopOrContinue->setText("Resume");
        } else if (msg == "GAME_RESUMED") {
            QMessageBox::information(this, "Game Resumed", "The game has been resumed.");
            isGamePaused = false;
            ui->stopOrContinue->setText("Pause");
        } else if (msg == "CARD_SELECTED") {
            qDebug() << "Card selected acknowledged by server.";

            if (inactivity)
                inactivity->stop();
            if (countDown)
                countDown->stop();

            QMessageBox::information(this, "Waiting", "Waiting for opponent to choose...");
        } else {
            qDebug() << "Unknown message: " << msg;
        }
    }
}
void GamePage::startChoosingCard()
{
    isMyTurn = true;
    for (QPushButton *btn : cards) {
        if (!btn->isEnabled()) {
            btn->setEnabled(true);
        }
    }

    QMessageBox::information(this, "Your Turn", "Please choose a card to play.");
    handleNotChoosingCard();
}
