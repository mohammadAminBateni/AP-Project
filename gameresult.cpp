#include "gameresult.h"
#include "menu.h"
#include "ui_gameresult.h"

GameResult::GameResult(QWidget *parent, QString gResult)
    : QWidget(parent)
    , ui(new Ui::GameResult)
    , result(gResult)
{
    ui->setupUi(this);
    if (gResult.toLower() == "win") {
        QString winAddress = ":/result/win.JPG";
        QPixmap w(winAddress);
        ui->result->setPixmap(
            w.scaled(ui->result->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        QString loseAddress = ":/result/lose.JPG";
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
    Menu *m = new Menu;
    m->show();
}
