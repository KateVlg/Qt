#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QDate>
#include <QUuid>
#include <QtCore>

enum sex
{
    man,
    woman
};
enum citizenship
{
    Russia,
    Ukraine,
    Poland,
    Germany,
    USSR
};

class person
{
public:
    person();
    person(QString pFIO,
    QString pfatherFIO,
    QString pmotherFIO,
    QDate pbirthday,
    bool pisALive,
    QDate pdeathDay,
    sex pSex,
    citizenship pcountry,
    bool pisMilitary);
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);
    bool operator>(const person &other)const;
    bool operator<(const person &other)const;
    bool operator>=(const person &other)const;
    bool operator<=(const person &other)const;
    bool operator==(const person &other)const;
    bool operator!=(const person &other)const;
    person* operator=(const person &other);

    static bool compare(const person& first, const person& second)
    {
        return first < second;
    }

private:
    QString FIO;
    QString fatherFIO;
    QString motherFIO;
    QDate birthday;
    bool isALive;
    QDate deathDay;
    sex Sex;
    citizenship country;
    bool isMilitary;
    QUuid id;

public:
    QString getFIO()const;
    void setFIO (QString value);

    QString getFatherFIO()const;
    void setFatherFIO (QString value);

    QString getMotherFIO()const;
    void setMotherFIO (QString value);

    QDate getBirthday()const;
    void setBirthday (QDate value);

    bool getIsAlive()const;
    void setIsAlive (bool value);

    QDate getDeathDay()const;
    void setDeathDay (QDate value);

    sex getSex()const;
    void setSex (sex value);

    citizenship getCountry()const;
    void setCountry (citizenship value);

    bool getIsMilitary()const;
    void setIsMilitary (bool value);

    QUuid getId()const;
    void setId (QUuid value);

    QString getParentsName()const;
};
#endif // PERSON_H
