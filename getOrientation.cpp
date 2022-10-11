#include "mainWindow.h"

#include <QFile>
#include <QDebug>
#include "exif.h"

// only for qDebug() below
#if 0
static const char * orients[] = {
                                 "undefined",
                                 "normal",
                                 "hori flip",
                                 "180 rot",
                                 "vert flip",
                                 "transpose",
                                 "90 rot",
                                 "transverse",
                                 "270 rot",
};
#endif

int MainWindow::getOrientation(const QString &filename, QString &qdatetime)
{
    char datetime[21];
    memset(datetime,0,21);
    QFile file(filename);
    int orientation = 0;
    if (file.open(QIODevice::ReadOnly) == true) {
        Exif exif;
        exif.readJpegSections(file, &orientation,datetime);
        file.close();
    }
//    qDebug() << "Orienta " << orients[orientation];
//    qDebug() << "datetime" << datetime;
    qdatetime = datetime;
    return orientation;
}
