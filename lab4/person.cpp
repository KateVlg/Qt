#include "person.h"
#include <QtCore>

person::person()
{
    FIO="Фамилия Имя Отчество";
    birthday=QDate().currentDate();
    isALive=true;
    Sex = man;
    country=Russia;
    id=QUuid::createUuid();
}
person::person(QString pFIO,
QString pfatherFIO,
QString pmotherFIO,
QDate pbirthday,
bool pisALive,
QDate pdeathDay,
sex pSex,
citizenship pcountry,
bool pisMilitary)
{
    FIO=pFIO;
    fatherFIO= pfatherFIO;
    motherFIO= pmotherFIO;
    birthday= pbirthday;
    isALive= pisALive;
    deathDay= pdeathDay;
    Sex= pSex;
    country= pcountry;
    isMilitary= pisMilitary;
    id=QUuid::createUuid();
}

QJsonObject person::toJson() const
{
    return {{"FIO", FIO}, {"fatherFIO", fatherFIO},{"motherFIO",motherFIO}, {"birthday", birthday.toString()},
        {"isALive", isALive}, {"deathDay", deathDay.toString()}, {"Sex", Sex}, {"country", country},
        {"isMilitary", isMilitary}, {"id", id.toString()}};
}

void person::fromJson(const QJsonObject &json)
{
    //if (json.contains("FIO") && json["FIO"].isString())
      FIO = json["FIO"].toString();
      fatherFIO = json["fatherFIO"].toString();
      motherFIO = json["motherFIO"].toString();
      birthday = QDate::fromString( json["birthday"].toString());
      isALive = json["isALive"].toBool();
      deathDay = QDate::fromString( json["deathDay"].toString());
      Sex = (sex)json["Sex"].toInt();
      country = (citizenship)json["country"].toInt();
      isMilitary = json["isMilitary"].toBool();
      id = QUuid::fromString(json["id"].toString());
}
QString person::getFIO()const
{
  return FIO;
}

void person::setFIO (QString value)
{
    FIO=value;
}

QString person::getFatherFIO()const
{
   return fatherFIO;
}
void person::setFatherFIO (QString value)
{
    fatherFIO=value;
}

QString person::getMotherFIO()const
{
    return motherFIO;
}
void person::setMotherFIO (QString value)
{
    motherFIO=value;
}

QDate person::getBirthday()const
{
    return birthday;
}
void person::setBirthday (QDate value)
{
    birthday=value;
}

bool person::getIsAlive()const
{
    return isALive;
}
void person::setIsAlive (bool value)
{
    isALive=value;
}

QDate person::getDeathDay()const
{
    return deathDay;
}
void person::setDeathDay (QDate value)
{
    deathDay=value;
}

sex person::getSex()const
{
    return Sex;
}
void person::setSex (sex value)
{
    Sex=value;
}

citizenship person::getCountry()const
{
    return country;
}
void person::setCountry (citizenship value)
{
    country=value;
}

bool person::getIsMilitary()const
{
    return isMilitary;
}
void person::setIsMilitary (bool value)
{
    isMilitary=value;
}

QUuid person::getId()const
{
    return id;
}

void person::setId (QUuid value)
{
    id=value;
}

QString person::getParentsName()const
{
    if (fatherFIO!=""&&motherFIO!="")
       {
            return fatherFIO+", "+motherFIO;
       }
    if (fatherFIO!="")
    {
        return fatherFIO;
    }

    return motherFIO;
}

bool person::operator>(const person &other)const
{
    if (birthday>other.birthday)
    {
        return true;
    }
    if (birthday<other.birthday)
    {
        return false;
    }

    return FIO>other.FIO;
}

bool person::operator<(const person &other)const
{
    if (birthday<other.birthday)
    {
        return true;
    }
    if (birthday>other.birthday)
    {
        return false;
    }

    return FIO<other.FIO;
}

bool person::operator>=(const person &other)const
{
    if (!(birthday<other.birthday))
    {
        return true;
    }
    if (birthday<other.birthday)
    {
        return false;
    }
}

bool person::operator<=(const person &other)const
{
    if (!(birthday>other.birthday))
    {
        return true;
    }
    if (birthday>other.birthday)
    {
        return false;
    }
}

bool person::operator==(const person &other)const
{
    if (birthday==other.birthday)
    {
        return true;
    }
    if (birthday!=other.birthday)
    {
        return false;
    }
    return FIO==other.FIO;
}

bool person::operator!=(const person &other)const
{
    if (birthday!=other.birthday)
    {
        return true;
    }
    if (birthday==other.birthday)
    {
        return false;
    }
    return FIO!=other.FIO;
}

person* person::operator=(const person &other)
{

    FIO=other.FIO;
    fatherFIO=other.fatherFIO;
    motherFIO=other.motherFIO;
    birthday=other.birthday;
    isALive=other.isALive;
    deathDay=other.deathDay;
    Sex=other.Sex;
    country=other.country;
    isMilitary=other.isMilitary;

    return this;
}
