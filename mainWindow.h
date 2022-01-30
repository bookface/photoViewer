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
#include <QMimeData>
#include <QDir>
#include <iostream>

class MyLabel : public QLabel {

    QFont _font;

  public:
    MyLabel(QWidget *parent = nullptr):QLabel(parent) {
        _font.setFamily("Times");
        _font.setPointSize(30);
        _font.setWeight(QFont::Bold);
        _font.setStyleStrategy(QFont::PreferAntialias);
        
    }
    QString _text;
    bool    _displayFileName = true;
    bool    _displayDirectory = true;

  protected:
    virtual void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        const QPixmap *pmap = this->pixmap();
        p.drawPixmap(rect(), *pmap);
        if (_displayFileName && _text.length()) {
        // create outline text
            p.setFont(_font);
            QPen pen;
            pen.setWidth(10);
            pen.setColor(Qt::black);
            p.setPen(pen);
            QPainterPath path;
            path.addText(0, 40, _font, _text);
            p.drawPath(path);
        // draw the normal text
            p.setPen(QPen(Qt::yellow));
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
    int  getOrientation(const QString &fileName);
    
    int _numFiles = 0;
    int _lastN = 0;
    int _currentN = 0;
    QList<QString> _names;
    QTimer *_imagetimer;
    bool    _ready = false;
    
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

    QByteArray _geometry;
    void setScreenSize(void);
    void resizeLabel(void);
    
    virtual void keyPressEvent(QKeyEvent *event) override
    {
        switch(event->key()) {
          case Qt::Key_Escape:
            exit(0);
            break;
          case Qt::Key_F:
            _fullscreen = !_fullscreen.toBool();
            if (!_fullscreen.toBool()) {
                restoreGeometry(_geometry);
            } else {
                _geometry = saveGeometry();
            }
            setScreenSize();
            break;
          case Qt::Key_N:
            nextImage();
            break;
          case Qt::Key_P:
            prevImage();
            break;
          default:
            QMainWindow::keyPressEvent(event);
        }
    }

    virtual bool event(QEvent *event) override
    {
        if (event->type() == QEvent::Resize) {
            resizeLabel();
        }
        return QMainWindow::event(event);
    }

    virtual void mousePressEvent(QMouseEvent *event) override;

    virtual void wheelEvent(QWheelEvent *event) override {
        QPoint numDegrees = event->angleDelta() / 8;
        if (numDegrees.y() < 0) {
            nextImage();
        } else if (numDegrees.y() > 0) {
            prevImage();
        }
    }
    
// be sure to call setAcceptDrops(true) in constructor
    virtual void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        }
    }

    virtual void dropEvent(QDropEvent *event) override {
        auto urls = event->mimeData()->urls();
        if (urls.size() == 0) return;
        auto dirname  = urls.first().path();
        auto fileName = urls.first().toLocalFile();
        qDebug() << dirname << fileName;
    // stars directorys with '/' for some reason
        if (dirname[0]=='/') {
            dirname.remove(0, 1); 
        }
        _names.clear();
        QDir qdir(dirname);
        if (qdir.exists()) {
            loadImagesFromDirectoryName(dirname);
            showImage();
        } else if (QFile::exists(fileName))
            loadImage(fileName);
    }

};


#endif //MAINWINDOW_H
