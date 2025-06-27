#ifndef MENU_H
#define MENU_H

#include <QTcpSocket>
#include <QWidget>
namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr,
                  const QString &username = "",
                  QTcpSocket *socket1 = nullptr,
                  QTcpSocket *socket2 = nullptr);
    ~Menu();
signals:
    void logoutRequest();

private slots:
    void on_logout_clicked();
    void readyRead();

private:
    Ui::Menu *ui;
    QString currentUsername;
    QTcpSocket sockets[2];
};

#endif // MENU_H
