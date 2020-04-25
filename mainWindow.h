//-*-c++-*-

#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QList>
#include <QTimer>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel *_label;
    QLabel *_info;
    void setFullScreen(void);
    void loadImagesFromDirectoryName(const QString &dirName);
    void showImageAndSleep(void);
    void showImage(void);
    void loadImage( const QString &fileName);
    int _numFiles = 0;
    int _lastN = 0;
    int _currentN = 0;
    int _secondsToShowImage = 30;
    bool _sleepMode = false;
    QList<QString> _names;
    QTimer *_imagetimer;
    
  protected:
    virtual void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Escape) {
            exit(0);
        }
    }
};


#endif //MAINWINDOW_H
