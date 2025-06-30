#include "gameresult.h"
#include "menu.h"
#include "ui_gameresult.h"

GameResult::GameResult(QWidget *parent, QString gResult, QTcpSocket *sock, QString u)
    : QWidget(parent)
    , ui(new Ui::GameResult)
    , result(gResult)
    , username(u)
    , socket(sock)
{
    ui->setupUi(this);
    if (gResult.toLower() == "win") {
        QString winAddress = ":/realWin.webp";
        QPixmap w(winAddress);
        ui->result->setPixmap(
            w.scaled(ui->result->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        QString loseAddress = ":/realLose.jpg";
        QPixmap l(loseAddress);
        ui->result->setPixmap(
            l.scaled(ui->result->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

GameResult::~GameResult()
{
    delete ui;
}

void GameResult::on_return_2_clicked()
{
    this->close();
    Menu *m = new Menu(nullptr, username, socket);
    m->show();
}
