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

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    srand (time(NULL));
    setStyleSheet("QMainWindow {background: 'black';}");
    auto screen = QApplication::desktop()->screenGeometry();
    resize(screen.width(),screen.height());

// create a label to show the picture
    _label = new QLabel(this);
    _label->setSizePolicy(QSizePolicy::Ignored,
                          QSizePolicy::Ignored);
    _label->setScaledContents(true);
    layout()->addWidget(_label);
    setFullScreen();

    _info = new QLabel(this);
    _info->setText("INFO LABEL");
    layout()->addWidget(_info);
    
// for testing, load a single image

#if 1

    QString path = QDir::currentPath();
    path += "/testimage.jpg";
    cout << "path " << qPrintable(path) << endl;
    loadImage(path);

#else

#ifdef _WIN32
    loadImagesFromDirectoryName("G:/share1920x1080");
#else
    loadImagesFromDirectoryName("/media/pi/Pictures/share1920x1080");
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
        cout << "Cant load " << qPrintable(fileName) << endl;
        return;
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
    delete _info;
}

#if 0

QImage image("(":/resources/images/images/01n.png");

QPainter* p = new QPainter(address of image to ctor);
p->setPen(Qt::black);
p->setFont(QFont("Arial", 12));
p->drawText(image.rect(), Qt::AlignCenter, "Text on Image/ Value"); //play around to display your text exactly how you need.

QLabel* Label1 = new QLabel();
Label->setPixmap(QPixmap::fromImage(image));
Label1->setAlignment(Qt::AlignLeft);



bool ImageSaver::save(const QString &path) const {    
  QImage image(img_);
  QPainter p;
  if (!p.begin(&image)) return false;

  p.setPen(QPen(Qt::red));
  p.setFont(QFont("Times", 12, QFont::Bold));
  p.drawText(image.rect(), Qt::AlignCenter, "Text");
  p.end();

  return image.save(path);
}


#endif
