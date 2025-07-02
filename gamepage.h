#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QLabel>
#include <QTcpSocket>
#include <QTime>
#include <QWidget>
#include "qpushbutton.h"
#include "swapdialog.h"
#include "user.h"
namespace Ui {
class GamePage;
}

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr,
                      const QString &username = "",
                      QTcpSocket *socket1 = nullptr);
    ~GamePage();
    void handleNotChoosingCard();
    void startChoosingCard();
signals:
    void updateTimerSignal(QString time);
public slots:
    void updateTimer(QString time);
private slots:
    void onCardClicked();
    void displayCards(const QStringList &cardList);
    void readyRead();
    void on_exit_clicked();

    void on_stopOrContinue_clicked();

    void on_swap_clicked();
    void handleDisconnection();

private:
    Ui::GamePage *ui;
    QVector<QPushButton *> cards;
    QTcpSocket *socket;
    QString username;
    QVector<QLabel *> labels;
    int currentSelection = 0;
    QTime elapsedTime;
    QTimer *timer1;
    QTimer *timer2;
    User u;
    bool isGamePaused;
    SwapDialog *sd = nullptr;
    QTimer *inactivity;
    QTimer *countDown;
    int notChoosingCard = 0;
    QTime inactivityElapsed;
    QTime countdownElapsed;
    int randNum;
    bool isMyTurn;
    int currentRound;
};

#endif // GAMEPAGE_H
