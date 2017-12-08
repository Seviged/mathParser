#ifndef MATHPARSER_H
#define MATHPARSER_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QString>
#include <QStringList>

#if defined(MATHPARSER_LIBRARY)
#  define MATHPARSERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MATHPARSERSHARED_EXPORT Q_DECL_IMPORT
#endif


class MATHPARSERSHARED_EXPORT MathParser
{

public:
    explicit MathParser();
    QStringList parse(QString);

private:
    QString parseString(QStringList, int);
    double parseBracets(QString);
    QString prepareString(QString);
    QString calculate(double, double, QString);
    QString validate(QString);

    int numOfIteration;
    int priority;
    QStringList allOpers;
    QStringList wrongSymbols;
    QStringList simpleOpers;
    QStringList sumOpers;
    QStringList divOpers;
    QStringList powOpers;
    QStringList mathOpers;
    QStringList mathNames;
    QStringList constNumbers;
    QStringList constNames;
};


#endif // MATHPARSER_H
