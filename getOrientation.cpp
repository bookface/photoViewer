#include "mainWindow.h"

#include <QFile>
#include <QDebug>
#include "exif.h"

static char * orients[] = {
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
