#include "gamepage.h"
#include "ui_gamepage.h"

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
