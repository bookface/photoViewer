// -*- coding: utf-8 -*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#include "mainWindow.h"
#include <QApplication>
#include <QLayout>
#include <QDirIterator>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QImageReader>
#include <QThreadPool>
#include <QMenu>
#include <QDir>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QTimer>
#include <QDebug>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QScreen>
#endif

#include <cstdlib>
#include <random>
#include <time.h>

#include "sleepyTime.h"

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// This loads the slide show as a background thread.  The main window
// will display an image while this is loading. Otherwise, it looks
// like the program is not working while the data is getting loaded.
// The flag "_ready" is set to true when this thread is complete.
//
class LoadSlideShow : public QRunnable {
    
    MainWindow *_win;
  public:
    LoadSlideShow(MainWindow *win) {
        _win = win;
    }

    void run() override {
        if (!_win->loadImagesFromDirectoryName(_win->_directory.toString())) {
            QMessageBox box;
            box.setText("No Images found");
            box.exec();
            exit(0);
        }

    // randomize list
        if (_win->_randomMode.toBool()) {
            std::random_device rng;
            std::mt19937 urng( rng() );
            std::shuffle(_win->_names.begin()
                         ,_win->_names.end(), urng);
        }
        _win->_ready = true;            // indicate we're done
    }
};


// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
MainWindow::MainWindow(QStringList args, QWidget *parent)
    : QMainWindow(parent)
{
// read settings from ini file
    QSettings settings("PhotoViewer.ini",QSettings::IniFormat);
    _directory = settings.value("Directory","./photos").value<QString>();
    _sleepMode = settings.value("SleepMode",_sleepMode).value<bool>();
    _secondsToShowImage = settings.value("DisplayTime"
                                         ,_secondsToShowImage).value<int>();
    _displayFileName = settings.value("DisplayFileName"
                                      ,_displayFileName).value<bool>();
    _hideCursor = settings.value("HideCursor",_hideCursor).value<bool>();
    _randomMode = settings.value("Random",_randomMode).value<bool>();
    _fullscreen = settings.value("Fullscreen",_fullscreen).value<bool>();
    
// override settings with command line options
    processCommandLine();

// hide the mouse cursor
    if (_hideCursor.toBool())
        QApplication::setOverrideCursor(Qt::BlankCursor);
    
// allow drag-drops
    setAcceptDrops(true);
    
// check for directory existance
    {
        QFileInfo f(_directory.toString());
        if (!f.exists()) {
            QMessageBox box;
            box.setText(QString("Photo Directory %1 Not Found")
                        .arg(_directory.toString()));
            box.exec();
            exit(0);
        }
    }

// Set the size of the screen
    setStyleSheet("QMainWindow {background: 'black';}");
    if (!_fullscreen.toBool()) {
    //
    // hmmm... if you don't do this, Qt creates a very
    // small initial window
    //
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        resize(screenGeometry.width() * 0.75
               ,screenGeometry.height() * 0.75);
    }

// create labels
    for (int i = 0; i < numLabels; ++i) {
        _label[i] = new MyLabel(this);
        _label[i]->_displayFileName = _displayFileName.toBool();
        _label[i]->setSizePolicy(QSizePolicy::Ignored,
                                 QSizePolicy::Ignored);
        _label[i]->setScaledContents(true);
    }
// set up window
    setScreenSize();

// load an initial image onto both labels while we
// wait for the list to be created
    QString path = QDir::currentPath();
    path += "/images/hello.png";
    QFileInfo fileInfo(path);
    if (fileInfo.exists()) {
        for (int i = 0; i < numLabels; ++i) {
            _currentLabel = i;
            loadImage(path);
        }
    }

// Load the initial slide show in a Thread.  QThreadPool automatically
// deletes the thread when it's finished
    
    QThreadPool::globalInstance()->start(new LoadSlideShow(this));
    
    _imagetimer = new QTimer;
    connect(_imagetimer,&QTimer::timeout, this,
                  [=]() {
                      if (_ready) {       // thread complete
                          showImage();    // display next image
                      }
                  });
    _imagetimer->start(_secondsToShowImage.toInt() * 1000);

    show();
    _geometry = saveGeometry();
    _opTimer = new QTimer(this);
    connect(_opTimer,&QTimer::timeout, this,
                  [=]() {
                      this->setOpacity();
                  });

    
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::nextImage(void)
{
    if (_names.size() == 0 ) return;    // single image display
    ++_lastN;
    if (_lastN == _names.size()) {
        _lastN = 0;
    }
    _currentN = _lastN;
    showImage();
    _imagetimer->start();

}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::prevImage(void)
{
    if (_names.size() == 0 ) return;    // single image display
    
    int newone = _lastN - 1;
    if (newone < 0) newone = _names.size()-1;
    _currentN = newone;
    showImage();
    _imagetimer->start();
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
bool MainWindow::loadImagesFromDirectoryName(const QString &dirName)
{
    QDirIterator it(dirName, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString name = it.next();
        QFileInfo info(name);
        if (!info.isDir()) {
            QImageReader qImageReader(name);
            if (qImageReader.canRead()) {
                _names.push_back(name);
            }
        }
    }
    return _names.size() > 0 ? true : false;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::showImage(void)
{
    if (_pause) return;

    _lastN = _currentN;
    _currentN++;

    if (_currentN >= _names.size()) {
    // end of list reached, re-sort list with new random call
        std::random_device rng;
        std::mt19937 urng( rng() );
        std::shuffle(_names.begin(), _names.end(), urng);
        _currentN = _lastN = 0;
    }
    
// move to next label
    _currentLabel = 1 - _currentLabel;
// start out new image with 0 opacity and fade in
    _label[_currentLabel]->setOpacity(0);
    loadImage(_names[_currentN]);

    _opacity = 1.0;
    _opTimer->start(100);

    if (_sleepMode.toBool()) {
        Sleep(_secondsToShowImage.toInt() * 1000 / 2);
    }
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::loadImage( const QString &fileName)
{
    MyLabel *label = _label[_currentLabel];
    
    QImage image;
    if (!image.load(fileName)) {
        label->_text = QString("Cant load %1").arg(fileName);
        return;
    }

//
// donn, Oct 10, 2022 - always load the file name
// into the _text variable since we can now toggle the
// display using the mouse pop up
//
    int orientation = getOrientation(fileName,_datetime);
//    qDebug() << "Orientation" << orientation << _datetime;
    _date = _datetime.left(10);

    {
        QFileInfo info(fileName);
    // just display the last directory + the file name
        QStringList list = fileName.split('/');
        int sz = list.size();
        if (sz > 2) {
            if (label->_displayDirectory) {
                label->_text = list[sz-2] + "/" + list[sz-1];
            } else {
                label->_text = list[sz-1];
            }
        } else {
            label->_text = fileName;
        }
        if (_date.length() > 0) {
            label->_text += " | " + _date;
        }
    }

// rotate image if needed
    QTransform rot;
    switch(orientation) {
      case 3:                           // 180 flip
        rot.rotate(180);
        break;
      case 6:                           // 90 rot
        rot.rotate(90);
        break;
      case 8:                           // 270 rot
        rot.rotate(-90);
        break;
      default:
        break;
    }

//
// Perform the Qt smoothing algorithm and apply
// the pixmap to the label.
//
// This really is only necessary for images that have not been pre-scaled
// to the screen size
//    
    if (image.width() > width()) {
        label->setPixmap(QPixmap::fromImage(image)
                         .transformed(rot).scaled(QSize(width(),height())
                                                  ,Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
    } else {
        label->setPixmap(QPixmap::fromImage(image).transformed(rot));
    }

    resizeLabel(label);
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::resizeLabel(MyLabel *label)
{
    if (label != nullptr) {
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
        float h = label->pixmap().height();
        float w = label->pixmap().width();
#else
        float h = label->pixmap()->height();
        float w = label->pixmap()->width();
#endif
        float scrWidth = width();
        float scrHeight = height();
        
        if (h < 0.01f ||
            w < 0.01f ||
            scrHeight < 0.01f ||
            scrWidth < 0.01f) return;   // prevent divide by zero
        float scaleH = scrHeight / h;
        float scaleW = scrWidth / w;
        float scale = scaleH;
        if (scaleW < scaleH) {
            scale = scaleW;
        }
        label->resize(w * scale,h * scale);
        float spaceLeftW = (scrWidth - w * scale) / 2.f;
        float spaceLeftH = (scrHeight - h * scale) / 2.f;
        label->move(spaceLeftW,spaceLeftH);
    }
}


// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::setScreenSize(void)
{
    if (_fullscreen.toBool()) {
        QScreen* screen = QGuiApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->geometry();
            resize(screenGeometry.width()
                   ,screenGeometry.height());
        }
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
        move(0,0);
    } else {
        setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
    }
    show();
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
MainWindow::~MainWindow()
{
    for (int i = 0; i < numLabels; ++i) {
        delete _label[i];
    }
    delete _imagetimer;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::setOpacity(void)
{
    if (_opacity <= 0.0) {              // stop fading
        _opTimer->stop();
    } else {
        _label[1-_currentLabel]->setOpacity(_opacity); // fade out last
        _label[_currentLabel]->setOpacity(1-_opacity); // fade in current
    }
    _opacity -= 0.1;
}
