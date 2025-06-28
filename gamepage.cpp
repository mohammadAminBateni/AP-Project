#include "gamepage.h"
#include <QTimer>
#include "mainwindow.h"
#include "menu.h"
#include "ui_gamepage.h"
#include "user.h"
GamePage::GamePage(QWidget *parent,
                   const QString &userName,
                   QTcpSocket *socket1,
                   QTcpSocket *socket2)
    : QWidget(parent)
    , ui(new Ui::GamePage)
    , sockets{socket1, socket2}
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
}

GamePage::~GamePage()
{
    delete ui;
}
void GamePage::displayCards(const QStringList &cardList)
{
    for (int i = 0; i < cardList.size() && i < cards.size(); ++i) {
        QString card = cardList[i];
        QString imagePath = ":/cards/" + card + ".JPG";

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

    QString card = btn->property("cardName").toString();

    QString message = "CHOOSE_CARD:" + card;
    sockets[0]->write(message.toUtf8()); // فرستادن پیام به سرور
    QString imagePath = ":/cards/" + card + ".JPG";
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
    sockets[0]->write(msg.toUtf8());

    this->close();
    Menu *m = new Menu(parentWidget(), username, sockets[0], sockets[1]);
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
                    sockets[0]->write(msg.toUtf8());
                    this->close();
                }
            });
            timer->start(1000);
            QString msg = "PAUSE_REQUEST:" + u.getUsername();
            sockets[0]->write(msg.toUtf8());
            ui->stopOrContinue->setText("Resume");
        }
    } else {
        isGamePaused = false;
        if (timer)
            timer->stop();
        QString msg = "RESUME_REQUEST:" + u.getUsername();
        sockets[0]->write(msg.toUtf8());
        ui->stopOrContinue->setText("Stop");
    }
}
void GamePage::updateTimer(QString time)
{
    ui->timer->display(time);
}

void GamePage::on_swap_clicked() {}
