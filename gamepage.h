#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QLabel>
#include <QTcpSocket>
#include <QTime>
#include <QWidget>
#include "qpushbutton.h"
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
                      QTcpSocket *socket1 = nullptr,
                      QTcpSocket *socket2 = nullptr);
    ~GamePage();
signals:
    void updateTimerSignal(QString time);
public slots:
    void updateTimer(QString time);
private slots:
    void onCardClicked();
    void displayCards(const QStringList &cardList);

    void on_exit_clicked();

    void on_stopOrContinue_clicked();

private:
    Ui::GamePage *ui;
    QVector<QPushButton *> cards;
    QTcpSocket *sockets[2];
    QString username;
    QVector<QLabel *> labels;
    int currentSelection = 0;
    QTime elapsedTime;
    User u;
    bool isGamePaused;
};

#endif // GAMEPAGE_H
