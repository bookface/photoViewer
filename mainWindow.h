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
    MyLabel *_label;
    void setFullScreen(void);
    bool loadImagesFromDirectoryName(const QString &dirName);
    void showImage(void);
    void loadImage( const QString &fileName);
    int _numFiles = 0;
    int _lastN = 0;
    int _currentN = 0;
    QList<QString> _names;
    QTimer *_imagetimer;

    QVariant _directory = "./photos";
    QVariant _secondsToShowImage = 30;
#ifdef __arm__                          // rasp pi default
    QVariant _sleepMode = true;
#else
    QVariant _sleepMode = false;
#endif
    QVariant _displayFileName = true;
    QVariant _hideCursor = true;
    
    void processCommandLine(void);
    
  protected:
    virtual void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Escape) {
            exit(0);
        }
    }
};


#endif //MAINWINDOW_H
