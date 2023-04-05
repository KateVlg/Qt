#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FIOvalidator.h"
#include <QList>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>
#include "person.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnAdd, SIGNAL (clicked()), this, SLOT(btnAddClicked()));
    connect(ui->btnFill, SIGNAL (clicked()), this, SLOT(btnFillClicked()));
    connect(ui->cbIsAlive, SIGNAL (toggled(bool)), this, SLOT(cbIsAliveCheked(bool)));
    connect(ui->btnDelete, SIGNAL (clicked()), this, SLOT(btnDeleteClicked()));
    connect(ui->btnSave, SIGNAL (clicked()), this, SLOT(btnSaveClicked()));
    connect(ui->tableWidget, SIGNAL (currentCellChanged(int,int,int,int)), this, SLOT (tableClicked(int,int,int,int)));
    connect(ui->btnCreateFile, SIGNAL (clicked()), this, SLOT(btnCreateFileClicked()));
    connect(ui->btnOpenFile, SIGNAL (clicked()), this, SLOT(btnOpenFileClicked()));
    connect(ui->btnSaveFile, SIGNAL (clicked()), this, SLOT(btnSaveFileClicked()));
    connect(ui->btnSaveAsFile, SIGNAL (clicked()), this, SLOT(btnSaveAsFileClicked()));
    EnableButtons(false);
    DBisEmpty=true;

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
int MainWindow::findIndex(QString FIO)
{
    for (int i=0; i<peopleDB.count(); i++)
    {
        if(peopleDB[i].getFIO()==FIO)
        {
            return i;
        }
    }
    return -1;
}
*/

bool MainWindow:: citizenshipUSSR(citizenship city, QDate deadDay)
{
    QDate dt=QDate(1990, 01, 01);
    if (deadDay<dt &&(city==Russia|| city==Ukraine))
    {
        ui->rbUSSR->setChecked(true);
        return true;
    }
    if(deadDay>dt && city==USSR)
    {
        return false;
    }

    return true;
}

bool MainWindow:: isCorrectFIO(QString FIO)
{
    QRegularExpression re(patternFIO);
    QRegularExpressionMatch match = re.match(FIO);
    if(!match.hasMatch()||FIO.length()>45)
    {
        return false;
    }

    return peopleDB.CheckUnique(FIO);
}

bool MainWindow:: isCorrectBthday(QDate bthDay,QString fatherFIO,QString motherFIO)
{
    return peopleDB.CheckBthday(bthDay,fatherFIO, motherFIO);
}

bool MainWindow:: isCorrectDeadDay(QDate BthDay, QDate DeadDay)
{
   QDate currentDay= QDate().currentDate();
    if(BthDay>DeadDay || DeadDay>currentDay)
    {
        return false;
    }
    return true;
}


void MainWindow:: fillCmbParents ()
{
    ui->cmbFather->clear();
    ui->cmbMother->clear();

    ui->cmbFather->addItem("не задан");
    ui->cmbMother->addItem("не задана");

        for (QList<person>::const_iterator iter=peopleDB.getconstBeginIter(); iter!=peopleDB.getconstEndIter(); ++iter)
    {
        if (currentId!=(*iter).getId() && (*iter).getFIO().length()>0)
        {
            if((*iter).getSex()==man)
            {
                ui->cmbFather->addItem((*iter).getFIO());
            }
            else
            {
                 ui->cmbMother->addItem((*iter).getFIO());
            }
        }
    }
}

void MainWindow::cbIsAliveCheked (bool checked)
{
    if (checked)
        ui->gbDead->hide();
    else
        ui->gbDead->show();
}

void MainWindow::btnAddClicked ()
{
    if (!isCorrectFIO("Фамилия Имя Отчество"))
    {
        return;
    }

    person p=person();
    currentId=peopleDB.append(p);
    showTableData();
    ui->tableWidget->selectRow(peopleDB.findIndexById(currentId));
    showItem();

}

void MainWindow::btnFillClicked ()
{
    peopleDB.FillList();

    showTableData();
    currentId=(*(peopleDB.getconstBeginIter())).getId();
    showItem();
    ui->tableWidget->selectRow(peopleDB.findIndexById(currentId));
}

void MainWindow::btnDeleteClicked ()
{
    if(peopleDB.isEmpty())
        return;
    peopleDB.remove(currentId);
    int temp=ui->tableWidget->currentRow();
    showTableData();
    if(ui->tableWidget->rowCount()<=temp)
    {
        temp--;
    }

    if(peopleDB.isEmpty())
        currentId=QUuid();
    ui->tableWidget->selectRow(temp);
    showItem();  
}

void MainWindow::tableClicked(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (currentRow!=previousRow && currentRow!=-1)
    {
        currentId=QUuid::fromString(ui->tableWidget->item(currentRow,4)->text());
        showItem();
    }
}

void MainWindow::showItem()
{
    //перезаполннение комбо боксов родителей
    fillCmbParents();

    if(peopleDB.isEmpty())
    {
        clearData();
        return;
    }

    person cur=peopleDB.record(currentId);

    //показать ФИО человека
    ui->editFIO->setText(cur.getFIO());

    //показать родителей
    ui->cmbFather->setCurrentText(cur.getFatherFIO());
    ui->cmbMother->setCurrentText(cur.getMotherFIO());

    //показать дату рождения
    ui->deBirthday->setDate(cur.getBirthday());

    //установить переключатель жизни
    ui->cbIsAlive->setChecked(cur.getIsAlive());

    //показать дату смерти
    ui->deDead->setDate(cur.getDeathDay());

    //показать пол
    if(cur.getSex()==man)
    {
        ui->rbMan->setChecked(true);
    }
    else
    {
        ui->rbWoman->setChecked(true);
    }

    //показать гражданство
    if (cur.getCountry()==Russia)
    {
        ui->rbRussia->setChecked(true);
    }
    else if (cur.getCountry()==Ukraine)
    {
            ui->rbUkraine->setChecked(true);
    }
    else if (cur.getCountry()==Poland)
    {
            ui->rbPoland->setChecked(true);
    }
    else if (cur.getCountry()==Germany)
    {
            ui->rbGermany->setChecked(true);
    }
    else
            ui->rbUSSR->setChecked(true);

    //установить переключатель военной службы
    ui->cbIsMilitary->setChecked(cur.getIsMilitary());
}

void MainWindow::btnSaveClicked ()
{
    if (peopleDB.isEmpty())
        return;
    //проверка на корректные данные
    if(!isCorrectFIO(ui->editFIO->text()))
    {
        QMessageBox::warning(this, "Внимание","Некорректно задано ФИО человека");
        return;
    }

    if(!isCorrectBthday(ui->deBirthday->date(),ui->cmbFather->currentText(),ui->cmbMother->currentText()))
    {
        QMessageBox::warning(this, "Внимание","Некорректно задан день рождения человека");
        return;
    }

    if(!isCorrectDeadDay(ui->deBirthday->date(),ui->deDead->date())&&!ui->cbIsAlive->isChecked())
    {
        QMessageBox::warning(this, "Внимание","Некорректно задана дата смерти человека");
        return;
    }

    //проверка на старое гражданство
    if (!ui->cbIsAlive->isChecked() && ui->rbRussia->isChecked())
    {
        citizenshipUSSR(Russia, ui->deDead->date());
    }
    else if (!ui->cbIsAlive->isChecked() && ui->rbUkraine->isChecked())
    {
        citizenshipUSSR(Ukraine, ui->deDead->date());
    }
    else if (ui->rbUSSR->isChecked())
    {
        if(!ui->cbIsAlive->isChecked() && !citizenshipUSSR(USSR, ui->deDead->date()))
        {
            QMessageBox::warning(this, "Внимание","Некорректно задано гражданство человека");
            return;
        }
    }

    citizenship country;
    if (ui->rbRussia->isChecked())
        country=Russia;
    else if (ui->rbUkraine->isChecked())
            country=Ukraine;
    else if (ui->rbPoland->isChecked())
            country=Poland;
    else if (ui->rbGermany->isChecked())
            country=Germany;
    else
        country=USSR;

    peopleDB.savePerson(currentId, ui->editFIO->text(), ui->cmbFather->currentText(), ui->cmbMother->currentText(), ui->deBirthday->date(),
            ui->cbIsAlive->isChecked(), ui->deDead->date(), ui->rbMan->isChecked(), country, ui->cbIsMilitary->isChecked());

    person cur = peopleDB.record(currentId);

    setRowData(ui->tableWidget->currentRow(), cur.getFIO(), cur.getBirthday().toString("dd.MM.yyyy"),
               cur.getIsAlive()?"":cur.getDeathDay().toString("dd.MM.yyyy"), cur.getParentsName(),cur.getId().toString());
    //showTableData();
    showItem();
}

void MainWindow::init()
{
    currentId=QUuid();
    clearData();
    createTable();
}

void MainWindow::createTable()
{
    ui->tableWidget->setColumnCount(5); // Указываем число колонок
    ui->tableWidget->setShowGrid(true); // Включаем сетку
    // Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем заголовки колонок
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << ("ФИО")
                                               << ("Дата рождения")
                                               << ("Дата смерти")
                                               << ("Родители")
                                               << ("ID"));
    ui->tableWidget->setColumnHidden(4,true);

    // Растягиваем последнюю колонку на всё доступное пространство
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setRowData (int row, QString FIO, QString birthday, QString deathDay, QString parentName, QString id)
{
    ui->tableWidget->setItem(row,0, new QTableWidgetItem(FIO));
    ui->tableWidget->setItem(row,1, new QTableWidgetItem(birthday));
    ui->tableWidget->setItem(row,2, new QTableWidgetItem(deathDay));
    ui->tableWidget->setItem(row,3, new QTableWidgetItem(parentName));
    ui->tableWidget->setItem(row,4, new QTableWidgetItem(id));
}

void MainWindow::showTableData()
{
    //int temp=ui->tableWidget->currentRow();
    //ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    int i=0;

    for (QList<person>::const_iterator iter=peopleDB.getconstBeginIter(); iter!=peopleDB.getconstEndIter(); ++iter)
    {
        // Вставляем строку
        ui->tableWidget->insertRow(i);
        // Далее забираем все данные из результата запроса и устанавливаем в остальные поля
        setRowData(i, (*iter).getFIO(), (*iter).getBirthday().toString("dd.MM.yyyy"),
                   (*iter).getIsAlive()?"":(*iter).getDeathDay().toString("dd.MM.yyyy"), (*iter).getParentsName(),(*iter).getId().toString());
        /*
        ui->tableWidget->setItem(i,0, new QTableWidgetItem((*iter).getFIO()));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem((*iter).getBirthday().toString()));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem((*iter).getIsAlive()?"":(*iter).getDeathDay().toString()));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem((*iter).getParentsName()));
        ui->tableWidget->setItem(i,4, new QTableWidgetItem((*iter).getId().toString()));
        */
        i++;
    }

    //if(ui->tableWidget->rowCount()<=temp)
        //temp--;
    //ui->tableWidget->selectRow(temp);
}

void MainWindow::clearData()
{
    ui->editFIO->setText("");
    ui->deBirthday->setDate(QDate().currentDate());
    ui->cbIsAlive->setChecked(true);
    ui->deDead->setDate(QDate().currentDate());
    ui->rbMan->setChecked(true);
    ui->rbRussia->setChecked(true);
    ui->cbIsMilitary->setChecked(false);

}

void MainWindow::EnableButtons(bool enable)
{
    ui->btnSave->setEnabled(enable);
    ui->btnFill->setEnabled(enable);
    ui->btnAdd->setEnabled(enable);
    ui->btnDelete->setEnabled(enable);
}

void MainWindow::btnCreateFileClicked ()
{

    QString str = QFileDialog::getSaveFileName(this, "Create File", "", "*.json");
    if (str.isEmpty())
        return;
    peopleDB=PersonDatabase(str);
    EnableButtons(true);
    init();
    DBisEmpty=false;
}

void MainWindow::btnOpenFileClicked ()
{
    QString str = QFileDialog::getOpenFileName(0, "Open File", "", "*.json");
    if (str.isEmpty())
        return;
    peopleDB=PersonDatabase(str);
    if(peopleDB.load(str))
    {
        EnableButtons(true);
        init();
        showTableData();
    }
    if(!peopleDB.isEmpty())
        currentId=(*(peopleDB.getconstBeginIter())).getId();
    else
        currentId=QUuid();
    ui->tableWidget->selectRow(peopleDB.findIndexById(currentId));
    DBisEmpty=false;
}

void MainWindow::btnSaveFileClicked ()
{
    if (DBisEmpty)
    {
        QMessageBox::warning(this, "Внимание","Нет существующей базы данных");
        return;
    }

    if(peopleDB.isModified())
    {
        peopleDB.save();
    }
    QMessageBox::warning(this, "Сообщение","База данных успешно сохранена");
}

void MainWindow::btnSaveAsFileClicked ()
{
    if (DBisEmpty)
    {
        QMessageBox::warning(this, "Внимание","Нет существующей базы данных");
        return;
    }

    QString str = QFileDialog::getSaveFileName(this, "Create File", "", "*.json");
    if (str.isEmpty())
        return;
    peopleDB.saveAs(str);

}
