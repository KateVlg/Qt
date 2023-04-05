#include "persondatabase.h"
#include "person.h"
#include <QList>

PersonDatabase::PersonDatabase(QString _path)
{
    path=_path;
}
PersonDatabase::PersonDatabase(){}

int PersonDatabase::count() const
{
    return people.count();
}

QUuid PersonDatabase::append(person &record)
{
    people.append(record);
    std::sort(people.begin(),people.end(),person::compare);

    Modified=true;

    return record.getId();

}

void PersonDatabase::remove(QUuid id)
{
    int temp;

    temp=findIndexById(id);

    if(temp!=-1)
    {
        people.removeAt(temp);
        Modified=true;
    }

}

int PersonDatabase::update(const person &record)
{
    Modified=true;
}

person PersonDatabase::record(QUuid id)
{
    int i=findIndexById(id);

    if(i!=-1)
    {
        return people[i];
    }

}

bool PersonDatabase::saveAs(QString filename)
{
    path=filename;
    return save();

}

bool PersonDatabase::save() const
{
    QFile saveFile(path);

       if (!saveFile.open(QIODevice::WriteOnly))
       {
           return false;
       }


       QJsonObject dbObject = toJson();
       saveFile.write(QJsonDocument(dbObject).toJson());

       return true;
}


QJsonObject PersonDatabase::toJson() const
{
    QJsonObject json;


   QJsonArray array;
   for (auto & p : people)
      array.append(p.toJson());
   json["people"] = array;
   return json;
}


bool PersonDatabase::load(QString filename)
{
    QFile loadFile(filename);

       if (!loadFile.open(QIODevice::ReadOnly))
       {
           return false;
       }

       QByteArray saveData = loadFile.readAll();

       QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

       fromJson(loadDoc.object());

       return true;
}

void PersonDatabase::fromJson(const QJsonObject &json)
{

        QJsonArray jsonpeople = json["people"].toArray();
        people.clear();

        for (int i = 0; i < jsonpeople.size(); ++i)
        {
            QJsonObject tmp = jsonpeople[i].toObject();
            person p;
            p.fromJson(tmp);
            people.append(p);
        }

}

void PersonDatabase::clear()
{
    people.clear();
    Modified=true;
}

bool PersonDatabase::isModified() const
{
    return Modified;
}

int PersonDatabase::findIndexById(QUuid id)
{
    QList<person>::const_iterator iter;
    int i=0;

    for (iter = people.constBegin(); iter!= people.constEnd(); ++iter)
    {

       if((*iter).getId()== id)
       {
          return i;
       }
              i++;
    }
    return -1;

}

bool PersonDatabase::isEmpty()
{
    return people.isEmpty();
}

QList<person>::const_iterator PersonDatabase::getconstBeginIter()
{
    return people.constBegin();
}

QList<person>::const_iterator PersonDatabase::getconstEndIter()
{
        return people.constEnd();
}

bool PersonDatabase::CheckUnique (QString FIO)
{
    QList<person>::const_iterator iter;

    for (iter = people.constBegin(); iter!= people.constEnd(); ++iter)
    {
        if (FIO==(*iter).getFIO())
            return false;
    }
    return true;
}

bool PersonDatabase:: CheckBthday(QDate bthDay,QString fatherFIO,QString motherFIO)
{
    QDate temp=bthDay.addYears(-15);
    QDate currentDay= QDate().currentDate();

    QList<person>::const_iterator iter;

    for (iter = people.constBegin(); iter!= people.constEnd(); ++iter)
    {
        if(fatherFIO==(*iter).getFIO()||motherFIO==(*iter).getFIO())
        {
            if((*iter).getBirthday()>temp|| bthDay>currentDay)
            {
                return false;
            }
        }
    }
    return true;
}


void PersonDatabase:: changeParentsNames (QString prevPersonFIO, QString newPersonFIO)
{
    QList<person>::iterator iter;

    for (iter = people.begin(); iter!= people.end(); ++iter)
    {
        if((*iter).getFatherFIO()==prevPersonFIO)
        {
            (*iter).setFatherFIO(newPersonFIO);
                Modified=true;
        }

        if((*iter).getMotherFIO()==prevPersonFIO)
        {
            (*iter).setMotherFIO(newPersonFIO);
                Modified=true;
        }
    }
}

void PersonDatabase:: savePerson(QUuid currentId, QString FIO, QString father, QString mother, QDate birthday,
                bool isAlive, QDate deathDay, bool Man, citizenship  country, bool military)
{
    //person cur = record(currentId);

    QList<person>::iterator iter;

    for (iter = people.begin(); iter!= people.end(); ++iter)
    {


        if((*iter).getId()==currentId)
        {
            if((*iter).getFIO().length()!=0)
            {//проверка на изменение фио человека
                    changeParentsNames((*iter).getFIO(),FIO);
            }

            //сохранение ФИО человека в текущем списке
            (*iter).setFIO(FIO);

            if (father!="не задан")
            {//сохранение фио отца, если задан
                (*iter).setFatherFIO(father);
            }
            else
            {//сохранение пустой строки, если отец не задан
                (*iter).setFatherFIO("");
            }

            if (mother!="не задана")
            {//сохранение фио матери, если задана
                (*iter).setMotherFIO(mother);
            }
            else
            {//сохранение пустой строки, если мать не задана
                (*iter).setMotherFIO("");
            }

            //сохранение дня рождения
            (*iter).setBirthday(birthday);
            //сохранение флага жизни
            (*iter).setIsAlive(isAlive);
            //сохранение даты смерти
            (*iter).setDeathDay(deathDay);

            if(Man)
                (*iter).setSex(man);
            else
                (*iter).setSex(woman);

            (*iter).setCountry(country);


            (*iter).setIsMilitary(military);

        }

    }

    Modified=true;
}


void PersonDatabase::FillList ()
{
    people.clear();


    people.append(person("Иванов Иван Василич","", "", QDate(1979,02,19), true,QDate(2021,12,16) , man, Russia, true));

    people.append(person("Иванова Марина Игоревна","", "Смирнова Алевтина Григорьевна", QDate(1985,04,30), true,QDate(2021,12,11) , woman, Russia, false));

    people.append(person("Иванов Иван Иваныч","Иванов Иван Василич", "Иванова Марина Игоревна", QDate(2021,12,16), true, QDate(2021,12,16), man, Russia, false));

    people.append(person("Петров Алексей Игоревич","", "",QDate(1971,06,07), true,QDate(2021,12,16) , man, Russia, false));

    people.append(person("Смирнова Алевтина Григорьевна","","", QDate(1935,04,30), false,QDate(1990,01,02) , woman, Ukraine, true));

    people.append(person("Петров Петр Алексеевич","Петров Алексей Игоревич", "",QDate(1999,04,30), false,QDate(1999,05,02) , man, Russia, false));

    people.append(person("Петрова Петра Алексеевич","Петров Алексей Игоревич", "",QDate(1999,04,30), true,QDate(2021,12,16) , man, Russia, false));

    people.append(person("Алексеева Илона Александровна","", "",QDate(2000,10,11), true,QDate(2021,12,16) , man, Russia, false));

    people.append(person("Алексеев Максим Иванович","Алексеев Иван Александрович", "",QDate(2009,07,17), true,QDate(2021,12,16) , man, Russia, false));

    people.append(person("Беляш Василиса Васьльевна","", "",QDate(1980,01,30), false, QDate(2020,01,10) , woman, Germany, false));

    std::sort(people.begin(),people.end(),person::compare);
}






