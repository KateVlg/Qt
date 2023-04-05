#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <person.h>
#include <QMainWindow>
#include "persondatabase.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//#define maxPersonCount 2;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //QList<person> people;
    PersonDatabase peopleDB;
    Ui::MainWindow *ui;
   // person people[maxPersonCount];
    QUuid currentId;
    bool DBisEmpty;
    void init();
    void fillCmbParents();
    bool isCorrectFIO(QString);
    bool isCorrectBthday(QDate,QString,QString);
    bool isCorrectDeadDay(QDate, QDate);
    void changeParentsNames (QString, QString);
    bool citizenshipUSSR(citizenship, QDate);
    int findIndex(QString);    
    void clearData();
    //const QString patternFIO = R"([А-ЯA-Z][а-яa-zА-ЯA-Z\-]{1,}\s[А-ЯA-Z][а-яa-zА-ЯA-Z\-]{1,}\s[А-ЯA-Z][а-яa-zА-ЯA-Z\-]{1,}$)";
    const QString patternFIO = R"(^([А-ЯA-Z]|[А-ЯA-Z][\x27а-яa-z]{1,}|[А-ЯA-Z][\x27а-яa-z]{1,}\-([А-ЯA-Z][\x27а-яa-z]{1,}|(оглы)|(кызы)))\040[А-ЯA-Z][\x27а-яa-z]{1,}(\040[А-ЯA-Z][\x27а-яa-z]{1,})$)";

    void showItem();
    void createTable();
    void showTableData ();
    void EnableButtons(bool);
    void setRowData (int, QString, QString, QString, QString, QString);


private slots:
     void btnAddClicked ();
     void btnFillClicked ();
     void cbIsAliveCheked (bool checked);
     void btnDeleteClicked ();
     void btnSaveClicked ();
     void tableClicked (int currentRow, int currentColumn, int previousRow, int previousColumn);
     void btnCreateFileClicked ();
     void btnOpenFileClicked ();
     void btnSaveFileClicked ();
     void btnSaveAsFileClicked ();

};



#endif // MAINWINDOW_H
