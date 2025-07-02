#include "user.h"
User::User(QString name1, QString family1, QString username1,
     QString password1, QString phone1, QString email1)
    : name(name1), family(family1), username(username1),
    password(password1), phone(phone1), email(email1)
{}


QString User::hashPassword(const QString &password)
{
    return QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha256).toHex();
}
