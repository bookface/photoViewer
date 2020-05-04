#include "mainWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QDirIterator>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QScreen>
#endif

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
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	QScreen* screen = QGuiApplication::primaryScreen();
	if (screen) {
		QRect screenGeometry = screen->geometry();
		resize(screenGeometry.width(), screenGeometry.height());
	}
#else
    auto screen = QApplication::desktop()->screenGeometry();
	resize(screen.width(), screen.height());
#endif

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
	 float scrWidth = 0.0f;
	 float scrHeight = 0.0f;
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	 QScreen* screen = QGuiApplication::primaryScreen();
	 if (screen) {
		 QRect screenGeometry = screen->geometry();
		 scrWidth = (float)screenGeometry.width();
		 scrHeight = (float)screenGeometry.height();
	 }
#else
     auto screen = QApplication::desktop()->screenGeometry();
     scrWidth = (float)screen.height();
     scrHeight = (float)screen.width();
#endif
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

void MainWindow::setFullScreen(void)
{
    setWindowFlags(Qt::FramelessWindowHint);
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	QScreen* screen = QGuiApplication::primaryScreen();
	if (screen) {
		QRect screenGeometry = screen->geometry();
		auto height = screenGeometry.height();
		move(0,0);
		resize(screenGeometry.width(), height);
	}
#else
    auto screen = QApplication::desktop()->screenGeometry();
    auto height = screen.height();
    move(0,0);
    resize(screen.width(), height);
#endif
    show();
}


MainWindow::~MainWindow()
{
    delete _label;
    delete _imagetimer;
}
