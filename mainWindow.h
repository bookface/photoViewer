//-*-c++-*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QVariant>
#include "roundRobin.h"

class QGraphicsOpacityEffect;
class QPaintEvent;
class QTimer;

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
class MyLabel : public QLabel {

    Q_OBJECT
    
    QFont _font;
    QGraphicsOpacityEffect *_effect;
    
  public:
    MyLabel(QWidget *parent = nullptr);
    void setOpacity(qreal v);

    QString _text;
    bool    _displayFileName = true;
    bool    _displayDirectory = true;
    
  protected:
    virtual void paintEvent(QPaintEvent *) override;
};

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
class MainWindow : public QMainWindow {

    Q_OBJECT

  public:
    MainWindow(QStringList args, QWidget *parent = nullptr);
    ~MainWindow();
    bool loadImagesFromDirectoryName(const QString &dirName);
    void showImage(void);
    bool loadImage( const QString &fileName);
    int  getOrientation(const QString &fileName, QString &datetime);
    void processCommandLine(void);
    void nextImage(void);
    void prevImage(void);
    void setOpacity(void);

    static const int _numLabels = 2;
    MyLabel *_label[_numLabels];
    int _currentLabel = 0;
    QString _datetime;
    QString _date;
    int _numFiles = 0;
    int _lastN = 0;
    int _currentN = 0;
    bool _pause = false;
    QList<QString> _names;
    QTimer *_imagetimer;
    QTimer *_opTimer;
    bool    _ready = false;
    bool    _usesqlite = false;
    
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
    QVariant _sqlite     = "";
    
  protected:

    void setScreenSize(void);
    void resizeLabel(MyLabel *);
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual bool event(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
// be sure to call setAcceptDrops(true) in constructor
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

    qreal      _opacity = 1.0;
    QByteArray _geometry;

// sqlite functions
    bool openDatabase(const QString &filename);
    unsigned char *getImage(void);
    RoundRobinList<QString> _rrList; // (10);

};


#endif //MAINWINDOW_H
