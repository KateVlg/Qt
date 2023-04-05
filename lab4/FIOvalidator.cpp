#include "FIOvalidator.h"
#include <QRegularExpression>

FIOvalidator::FIOvalidator(QObject* parent)
    : QValidator (parent)
{

}

FIOvalidator::~FIOvalidator()
{

}

int countSpace=0;

QValidator::State FIOvalidator::validate(QString &string, int &pos) const
{
    int pos1=pos-1;

    //Проверка длины строки
    if (string.count()>45) return QValidator::Invalid;

    if (pos1==0||((pos1!=0) && (string[pos1-1].isSpace())))
    {
        if (!string[pos1].isUpper())
            return QValidator::Invalid;
    }

    else if (string[pos1-1].isUpper()||(string[pos1-1].isLower()&&!string[pos1].isSpace()))
    {
        if(!string[pos1].isLower())
            return QValidator::Invalid;
    }

   /* else if (string[pos1].isSpace())
    {
        countSpace++;
        if (countSpace>2)
            return QValidator::Invalid;
    }*/

    else if (!string[pos1].isUpper() && !string[pos1].isLower() && !string[pos1].isSpace())
        return QValidator::Invalid;

    return QValidator::Acceptable;
}
