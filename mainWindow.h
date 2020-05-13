//-*-c++-*-

#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QList>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <iostream>

class MyLabel : public QLabel {
  public:
    MyLabel(QWidget *parent = nullptr):QLabel(parent) {}
    QString _text;
    bool    _displayFileName = true;
    bool    _displayDirectory = true;

  protected:
    virtual void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        const QPixmap *pmap = this->pixmap();
        p.drawPixmap(rect(), *pmap);
        if (_displayFileName && _text.length()) {
            p.setPen(QPen(Qt::yellow));
            p.setFont(QFont("Times", 30, QFont::Bold));
            p.drawText(rect(), Qt::AlignTop, _text);
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QStringList args, QWidget *parent = 0);
    ~MainWindow();
    MyLabel *_label = nullptr;

    bool loadImagesFromDirectoryName(const QString &dirName);
    void showImage(void);
    void loadImage( const QString &fileName);

    int _numFiles = 0;
    int _lastN = 0;
    int _currentN = 0;
    QList<QString> _names;
    QTimer *_imagetimer;

    void processCommandLine(void);
    void nextImage(void);
    void prevImage(void);
    

//
// Program options - QVariant's makes it simplier to load
// from the command line or the configuration file
//
    QVariant _directory = "./photos";
    QVariant _secondsToShowImage = 30;
#ifdef __arm__                          // rasp pi default
    QVariant _sleepMode = true;
#else
    QVariant _sleepMode = false;
#endif
    QVariant _displayFileName = true;
    QVariant _hideCursor = true;
    QVariant _randomMode = true;
    QVariant _fullscreen = true;
    
  protected:

    void setScreenSize(void);
    void scaleImage(void);
    
    virtual void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Escape) {
            exit(0);
        }
        if (event->key() == Qt::Key_F) {
            _fullscreen = !_fullscreen.toBool();
            setScreenSize();
        }
    }

    virtual bool event(QEvent *event)
    {
        if (event->type() == QEvent::Resize) {
            scaleImage();
        }
        return QMainWindow::event(event);
    }

    virtual void wheelEvent(QWheelEvent *event) {
        QPoint numDegrees = event->angleDelta() / 8;
        if (numDegrees.y() < 0) {
            nextImage();
        } else if (numDegrees.y() > 0) {
            prevImage();
        }

    }
};


#endif //MAINWINDOW_H
