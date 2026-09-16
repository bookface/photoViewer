//-*-c++-*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#pragma once
#include <QList>
#include <QString>

bool writeListToFile(const QList<QString>& list, const QString& filename);
QList<QString> readListFromFile(const QString& filename);

