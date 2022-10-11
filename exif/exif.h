
#ifndef EXIF_H
#define EXIF_H

// This implementation is based on http://www.sentex.net/~mwandel/jhead/
// Rewritten and published in public domain like
// the original code by http://imonad.com

#include <QList>
#include <QByteArray>

typedef unsigned int  rint32u;

class Exif {

  public:
    Exif();
    ~Exif();
    int getExifOrientation(QFile &file, int *Orientation);
    int readJpegFile(QFile &file, int *Orientation);
    int readJpegSections(QFile &file, int *Orientation,char *datetime);
    int processEXIF(QByteArray *barr, int itemlen, int *Orientation,char *datetime);
    int processEXIFDir(const char *dirStart
                       ,const char *offsetBase
                       ,rint32u size
                       ,rint32u nesting
                       ,int MotorolaOrder
                       ,int *numOrientations
                       ,int *Orientation
                       ,char *datetime);
};



#endif // EXIF_H
