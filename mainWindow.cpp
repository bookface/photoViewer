#include "mainWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QDirIterator>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

#include <cstdlib>
#include <iostream>
#include <time.h>

#include "sleepyTime.h"

// #define TEST_SINGLE_IMAGE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSettings settings("PhotoViewer.ini",QSettings::IniFormat);
    QString   directory = settings.value("Directory","./photos").value<QString>();
    bool      _sleepMode = settings.value("SleepMode",false).value<bool>();
    int       _secondsToShowImage = settings.value("DisplayTime",30).value<int>();
    bool      displayFileName = settings.value("DisplayFileName",true).value<bool>();

    {
        QFileInfo f(directory);
        if (!f.exists()) {
            QMessageBox box;
            box.setText(QString("Photo Directory %1 Not Found").arg(directory));
            box.exec();
            exit(0);
        }
    }

// init random number generator
    srand (time(NULL));

// fill the whole screen with the main window
    setStyleSheet("QMainWindow {background: 'black';}");
    auto screen = QApplication::desktop()->screenGeometry();
    resize(screen.width(),screen.height());

// create a label to show the picture
    _label = new MyLabel(this);
    _label->_displayFileName = displayFileName;
    _label->setSizePolicy(QSizePolicy::Ignored,
                          QSizePolicy::Ignored);
    _label->setScaledContents(true);
    layout()->addWidget(_label);
    
// for testing, load a single image

#ifdef TEST_SINGLE_IMAGE

    QString path = QDir::currentPath();
    path += "/testimage.jpg";
    loadImage(path);

#else  // load slideshow

    if (!loadImagesFromDirectoryName(directory)) {
        QMessageBox box;
        box.setText("No Images found");
        box.exec();
        exit(0);
    }

    setFullScreen();                    // remove window decorations
    showImage();                        // load initial image

// create timer to display images
    _imagetimer = new QTimer;
    connect(_imagetimer,&QTimer::timeout, this,
            [=]() {
                showImage();
            });
    _imagetimer->start(_secondsToShowImage * 1000);

#endif
    
}

bool MainWindow::loadImagesFromDirectoryName(const QString &dirName)
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
    return _names.size() > 0 ? true : false;
}

void MainWindow::showImage(void)
{
    _lastN = _currentN;
    int n = rand() % _names.size();
    loadImage(_names[n]);
    _currentN = n;
// sleep for half the display time
    if (_sleepMode) {
        Sleep(_secondsToShowImage * 1000 / 2);
    }
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
     float scaleH = (float)screen.height() / h;
     float scaleW = (float)screen.width() / w;
     float scale = scaleH;
     if (scaleW < scaleH) {
         scale = scaleW;
     }
     _label->resize(w * scale,h * scale);
     float spaceLeftW = (screen.width() - w * scale) / 2.f;
     float spaceLeftH = (screen.height() - h * scale) / 2.f;
     _label->move(spaceLeftW,spaceLeftH);
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
    delete _imagetimer;
}

