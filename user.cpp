#include "user.h"
#include <QCryptographicHash>
#include <QStringList>
int User::userCount = 0;
int User::getUserCount()
{
    return userCount;
}
User::User()
{
    firstName = "";
    lastName = "";
    phone = "";
    gmail = "";
    username = "";
    password = "";
    userCount++;
}

User::User(QString firstName,
           QString lastName,
           QString phone,
           QString gmail,
           QString username,
           QString password)
{
    firstName = this->firstName;
    lastName = this->lastName;
    phone = this->phone;
    username = this->username;
    password = this->password;
    gmail = this->gmail;
    userCount++;
}
QString User::hashPassword(const QString &password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}
QTextStream &operator<<(QTextStream &out, User &u)
{
    out << u.firstName << "|" << u.lastName << "|" << u.gmail << "|" << u.phone << "|" << u.username
        << "|" << u.hashPassword(u.password) << "\n";
    return out;
}
QTextStream &operator>>(QTextStream &in, User &u)
{
    QString line = in.readLine();
    QStringList parts = line.split("|");

    if (parts.size() >= 5) {
        u.firstName = parts[0];
        u.lastName = parts[1];
        u.gmail = parts[2];
        u.phone = parts[3];
        u.username = parts[4];
        u.password = parts[5];
    }
    return in;
}

User::User(QString username, QString password)
{
    username = this->username;
    password = this->password;
}
