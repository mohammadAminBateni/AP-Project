#include "user.h"
#include <QCryptographicHash>
#include <QStringList>
int User::userCount = 0;
int User::getUserCount()
{
    return userCount;
}
void User::setFirstName(const QString &newFirstName)
{
    firstName = newFirstName;
}

void User::setLastName(const QString &newLastName)
{
    lastName = newLastName;
}

void User::setPhone(const QString &newPhone)
{
    phone = newPhone;
}

void User::setGmail(const QString &newGmail)
{
    gmail = newGmail;
}

void User::setUsername(const QString &newUsername)
{
    username = newUsername;
}

int User::getPauseRequests() const
{
    return pauseRequests;
}

void User::setPauseRequests(int newPauseRequests)
{
    pauseRequests = newPauseRequests;
}

QString User::getFirstName() const
{
    return firstName;
}

QString User::getLastName() const {}

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
    this->firstName = firstName;
    this->lastName = lastName;
    this->phone = phone;
    this->gmail = gmail;
    this->username = username;
    this->password = password;
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
QString User::getPassword()
{
    return password;
}
QString User::getUsername()
{
    return username;
}

QString User::getPhone()
{
    return phone;
}

void User::setPassword(QString password)
{
    this->password = password;
}
QString User::toString() const
{
    return QString("%1|%2|%3|%4|%5|%6")
        .arg(username)
        .arg(firstName)
        .arg(lastName)
        .arg(phone)
        .arg(gmail)
        .arg(password);
}
User User::fromString(const QString &str)
{
    User user;
    QStringList parts = str.split('|');

    if (parts.size() >= 6) {
        user.setUsername(parts[0]);
        user.setFirstName(parts[1]);
        user.setLastName(parts[2]);
        user.setPhone(parts[3]);
        user.setGmail(parts[4]);
        user.setPassword(parts[5]);
    }

    return user;
}
