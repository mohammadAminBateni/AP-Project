#ifndef USER_H
#define USER_H
#include <QString>
#include <QTextStream>

class User
{
    QString firstName;
    QString lastName;
    QString phone;
    QString gmail;
    QString password;
    QString username;
    static int userCount;

public:
    User();
    User(QString firstName,
         QString lastName,
         QString phone,
         QString gmail,
         QString username,
         QString password);
    User(QString username, QString password);
    friend QTextStream &operator<<(QTextStream &out, User &u);
    friend QTextStream &operator>>(QTextStream &in, User &u);
    QString hashPassword(const QString &password);
    static int getUserCount();
    QString getPassword();
    QString getUsername();
    QString getPhone();
};

#endif // USER_H
