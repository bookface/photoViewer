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

int MainWindow::getOrientation(const QString &filename)
{
    QFile file(filename);
    int orientation = 0;
    if (file.open(QIODevice::ReadOnly) == true) {
        Exif exif;
        exif.readJpegSections(file, &orientation);
        file.close();
    }
//    qDebug() << "Orienta " << orients[orientation];
    return orientation;
}
