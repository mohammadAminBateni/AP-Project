#include "swapdialog.h"
#include "ui_swapdialog.h"

SwapDialog::SwapDialog(QWidget *parent, QTcpSocket *s)
    : QDialog(parent)
    , ui(new Ui::SwapDialog)
    , socket(s)
{
    ui->setupUi(this);
}

SwapDialog::~SwapDialog()
{
    delete ui;
}

QString SwapDialog::getSelectedCard()
{
    return selectedCard;
}

void SwapDialog::on_buttonBox_accepted()
{
    QString message = "SWAP_RESPONSE:YES";
    socket->write(message.toUtf8());
}

void SwapDialog::on_buttonBox_rejected()
{
    QString message = "SWAP_RESPONSE:NO";
    socket->write(message.toUtf8());
}
void SwapDialog::setSelectedCard(const QString &cardName)
{
    selectedCard = cardName;
}
