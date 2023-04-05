#ifndef FIOVALIDATOR_H
#define FIOVALIDATOR_H

#include <QValidator>
#include <ctype.h>

class FIOvalidator: public QValidator
{
    Q_OBJECT
public:
    FIOvalidator(QObject *parent);
    ~FIOvalidator();

    QValidator::State validate(QString & string, int & pos) const;

private:
     const QString patternFIO = R"(^([А-ЯA-Z]|[А-ЯA-Z][\x27а-яa-z]{1,}|[А-ЯA-Z][\x27а-яa-z]{1,}\-([А-ЯA-Z][\x27а-яa-z]{1,}|(оглы)|(кызы)))\040[А-ЯA-Z][\x27а-яa-z]{1,}(\040[А-ЯA-Z][\x27а-яa-z]{1,})$)";

};

#endif // FIOVALIDATOR_H
