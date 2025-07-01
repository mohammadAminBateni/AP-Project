#ifndef USER_H
#define USER_H

#include <QString>

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

    User(QString name, QString family, QString username,
         QString password, QString phone, QString email)
        : name(name), family(family), username(username),
        password(password), phone(phone), email(email)
    {}
};

#endif // USER_H
