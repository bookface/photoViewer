#include "mainWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QDirIterator>
#include <QDebug>
#include <cstdlib>
#include <iostream>
#include <time.h>

#include "sleepyTime.h"

//
// Where photos are
//
#define WINDOWS_PHOTO_DIR "G:/share1920x1080"
#define UNIX_PHOTO_DIR    "/media/pi/Pictures/share1920x1080"

// #define TEST_SINGLE_IMAGE

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    srand (time(NULL));
    setStyleSheet("QMainWindow {background: 'black';}");
    auto screen = QApplication::desktop()->screenGeometry();
    resize(screen.width(),screen.height());

// create a label to show the picture
    _label = new MyLabel(this);
    _label->setSizePolicy(QSizePolicy::Ignored,
                          QSizePolicy::Ignored);
    _label->setScaledContents(true);
    layout()->addWidget(_label);
    setFullScreen();

// for testing, load a single image

#ifdef TEST_SINGLE_IMAGE

    QString path = QDir::currentPath();
    path += "/testimage.jpg";
    cout << "path " << qPrintable(path) << endl;
    loadImage(path);

#else  // load slideshow

#ifdef _WIN32
    loadImagesFromDirectoryName(WINDOWS_PHOTO_DIR);
#else
    loadImagesFromDirectoryName(UNIX_PHOTO_DIR);
    _sleepMode = true;
#endif

    showImage();
    _imagetimer = new QTimer;
    if (_sleepMode) {
        connect(_imagetimer, &QTimer::timeout, this,
                [=]() {
                    showImageAndSleep();
                });
    } else {
        connect(_imagetimer,&QTimer::timeout, this,
                [=]() {
                    showImage();
                });
    }
    _imagetimer->start(_secondsToShowImage * 1000);

#endif
    
}

void MainWindow::loadImagesFromDirectoryName(const QString &dirName)
{
    QDirIterator it(dirName, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString name = it.next();
        QFileInfo info(name);
        if (!info.isDir()) {
        // qDebug() << it.next();
            _names.push_back(name);
        }
    }
}

void MainWindow::showImageAndSleep(void)
{
    showImage();
    Sleepy(_secondsToShowImage / 2 ,0);
}

void MainWindow::showImage(void)
{
    _lastN = _currentN;
    int n = rand() % _names.size();
    loadImage(_names[n]);
    _currentN = n;
}

void MainWindow::loadImage( const QString &fileName)
{
    QImage image;
    if (!image.load(fileName)) {
        _label->_text = QString("Cant load %1").arg(fileName);
        return;
    }
    bool showFileName(true);

    if (showFileName){
        QFileInfo info(fileName);
    // just display the last directory + the file name
        QStringList list = fileName.split('/');
        int sz = list.size();
        if (sz > 2) {
            _label->_text = list[sz-2] + "/" + list[sz-1];
        } else {
            _label->_text = fileName;
            
        }
    }
     _label->setPixmap(QPixmap::fromImage(image));
     float h = _label->pixmap()->height();
     float w = _label->pixmap()->width();
     auto screen = QApplication::desktop()->screenGeometry();
     auto scaleH = (float)screen.height() / h;
     auto scaleW = (float)screen.width() / w;
     auto scale = scaleH;
     if (scaleW < scaleH) {
         scale = scaleW;
     }
     _label->resize(w * scale,h * scale);
     float spaceLeft = (screen.width() - w * scale) / 2.0;
     _label->move(spaceLeft,0);
}

void MainWindow::setFullScreen(void)
{
    setWindowFlags(Qt::FramelessWindowHint);
    auto screen = QApplication::desktop()->screenGeometry();
    auto height = screen.height();
    move(0,0);
    resize(screen.width(),height);
    show();
}


MainWindow::~MainWindow()
{
    delete _label;
}

