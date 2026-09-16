//-*-c++-*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// read/write the list of photo file names to a file

#include <QFile>
#include <QTextStream>

#include "nameList.h"

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
bool writeListToFile(const QList<QString>& list, const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    for (const QString& str : list)
        out << str << '\n';

    file.close();
    return true;
}


// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
QList<QString> readListFromFile(const QString& filename)
{
    QList<QString> list;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return list;

    QTextStream in(&file);

    while (!in.atEnd())
        list.append(in.readLine());

    return list;
}
