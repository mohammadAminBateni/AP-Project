#ifndef USER_H
#define USER_H

#include <QString>
#include "QCryptographicHash"

class User
{
public:
    QString name;
    QString family;
    QString username;
    QString password;
    QString phone;
    QString email;
    User(){}

    User(QString name1, QString family1, QString username1,
         QString password1, QString phone1, QString email1);

    QString hashPassword(const QString &password);
};

#endif // USER_H
