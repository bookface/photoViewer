#include "mainWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QDirIterator>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QImageReader>

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QScreen>
#endif

#include <cstdlib>
#include <random>
#include <time.h>

#include "sleepyTime.h"

// #define TEST_SINGLE_IMAGE

MainWindow::MainWindow(QStringList args, QWidget *parent)
    : QMainWindow(parent)
{
    QSettings settings("PhotoViewer.ini",QSettings::IniFormat);
    _directory = settings.value("Directory","./photos").value<QString>();
    _sleepMode = settings.value("SleepMode",_sleepMode).value<bool>();
    _secondsToShowImage = settings.value("DisplayTime",_secondsToShowImage).value<int>();
    _displayFileName = settings.value("DisplayFileName",_displayFileName).value<bool>();
    _hideCursor = settings.value("HideCursor",_hideCursor).value<bool>();
    _randomMode = settings.value("Random",_randomMode).value<bool>();
    _fullscreen = settings.value("Fullscreen",_fullscreen).value<bool>();
    
// override settings with command line options
    processCommandLine();

// hide the mouse cursor
    if (_hideCursor.toBool())
        QApplication::setOverrideCursor(Qt::BlankCursor);
    
#if 0
    if (args.size() > 1) directory = args.at(1);
    if (args.size() > 2) _secondsToShowImage = args.at(2).toInt();
    if (args.size() > 3) displayFileName = (args.at(3).toInt() != 0);
#endif
    
    {
        QFileInfo f(_directory.toString());
        if (!f.exists()) {
            QMessageBox box;
            box.setText(QString("Photo Directory %1 Not Found").arg(_directory.toString()));
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
    setScreenSize();

// create a label to show the picture
    _label = new MyLabel(this);
    _label->_displayFileName = _displayFileName.toBool();
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

    if (!loadImagesFromDirectoryName(_directory.toString())) {
        QMessageBox box;
        box.setText("No Images found");
        box.exec();
        exit(0);
    }

// randomize list
    if (_randomMode.toBool()) {
        std::random_device rng;
        std::mt19937 urng( rng() );
        std::shuffle(_names.begin(), _names.end(), urng);
    }

    showImage();                        // load initial image

// create timer to display images
    _imagetimer = new QTimer;
    connect(_imagetimer,&QTimer::timeout, this,
            [=]() {
                showImage();
            });
    _imagetimer->start(_secondsToShowImage.toInt() * 1000);

#endif

}

void MainWindow::nextImage(void)
{
    ++_lastN;
    if (_lastN == _names.size()) {
        _lastN = 0;
    }
    _currentN = _lastN;
    showImage();
    _imagetimer->start();
}

void MainWindow::prevImage(void)
{
    int newone = _lastN - 1;
    if (newone < 0) newone = _names.size()-1;
    _currentN = newone;
    showImage();
    _imagetimer->start();
}

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

void MainWindow::showImage(void)
{
    _lastN = _currentN;
    _currentN++;

    if (_currentN >= _names.size()) {
// end of list reached, re-sort list with new random call
        std::random_device rng;
        std::mt19937 urng( rng() );
        std::shuffle(_names.begin(), _names.end(), urng);
        _currentN = _lastN = 0;
    }

	loadImage(_names[_currentN]);

// sleep for half the display time
    if (_sleepMode.toBool()) {
        Sleep(_secondsToShowImage.toInt() * 1000 / 2);
    }
}

void MainWindow::loadImage( const QString &fileName)
{
    QImage image;
    if (!image.load(fileName)) {
        _label->_text = QString("Cant load %1").arg(fileName);
        return;
    }

    if (_label->_displayFileName) {
        QFileInfo info(fileName);
    // just display the last directory + the file name
        QStringList list = fileName.split('/');
        int sz = list.size();
        if (sz > 2) {
            if (_label->_displayDirectory) {
                _label->_text = list[sz-2] + "/" + list[sz-1];
            } else {
                _label->_text = list[sz-1];
            }
        } else {
            _label->_text = fileName;

        }
    }
     _label->setPixmap(QPixmap::fromImage(image));
     scaleImage();
     
}

void MainWindow::scaleImage(void)
{
    if (_label != nullptr) {
        float h = _label->pixmap()->height();
        float w = _label->pixmap()->width();
        float scrWidth = width();
        float scrHeight = height();
        
        if (h < 0.01f || w < 0.01f || scrHeight < 0.01f || scrWidth < 0.01f) return;  // prevent divide by zero
        float scaleH = scrHeight / h;
        float scaleW = scrWidth / w;
        float scale = scaleH;
        if (scaleW < scaleH) {
            scale = scaleW;
        }
        _label->resize(w * scale,h * scale);
        float spaceLeftW = (scrWidth - w * scale) / 2.f;
        float spaceLeftH = (scrHeight - h * scale) / 2.f;
        _label->move(spaceLeftW,spaceLeftH);
    }
}

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

MainWindow::~MainWindow()
{
    delete _label;
    delete _imagetimer;
}
