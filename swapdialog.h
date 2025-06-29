#ifndef SWAPDIALOG_H
#define SWAPDIALOG_H

#include <QDialog>
#include "qtcpsocket.h"

namespace Ui {
class SwapDialog;
}

class SwapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SwapDialog(QWidget *parent = nullptr, QTcpSocket * = nullptr);
    ~SwapDialog();
    QString getSelectedCard();
    void setSelectedCard(const QString &cardName);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SwapDialog *ui;
    QTcpSocket *socket;
    QString selectedCard;
};

#endif // SWAPDIALOG_H
