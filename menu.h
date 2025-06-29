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
                  QTcpSocket *socket = nullptr);
    ~Menu();
signals:
    void logoutRequest();

private slots:
    void on_logout_clicked();
    void readyRead();

    void on_hsitory_clicked();

private:
    Ui::Menu *ui;
    QString currentUsername;
    QTcpSocket *socket;
};

#endif // MENU_H
