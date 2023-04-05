#ifndef PERSONDATABASE_H
#define PERSONDATABASE_H

#include "person.h"

class PersonDatabase
{
public:
    PersonDatabase(QString);
    PersonDatabase();

    int count() const;
    QUuid append(person &record);
    void remove(QUuid id);
    int update(const person &record);
    person record(QUuid id);
    bool saveAs(QString filename);
    bool save() const;
    bool load(QString filename);
    void clear();
    bool isModified() const;
    int findIndexById(QUuid);
    bool isEmpty();
    QList<person>::const_iterator getconstBeginIter();
    QList<person>::const_iterator getconstEndIter();
    bool CheckUnique (QString);
    bool CheckBthday(QDate,QString,QString);
    void changeParentsNames (QString, QString);
    void FillList ();
    void savePerson(QUuid currentId, QString, QString, QString, QDate, bool, QDate, bool, citizenship, bool);


private:
    QList<person> people;
    bool Modified;
    QString path;
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);


};

#endif // PERSONDATABASE_H
