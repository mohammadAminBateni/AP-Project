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
    int pauseRequests;

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
    void setPassword(QString password);
    void setFirstName(const QString &newFirstName);
    void setLastName(const QString &newLastName);
    void setPhone(const QString &newPhone);
    void setGmail(const QString &newGmail);
    void setUsername(const QString &newUsername);
    QString toString() const;
    static User fromString(const QString &str);
    int getPauseRequests() const;
    void setPauseRequests(int newPauseRequests);
};

#endif // USER_H
