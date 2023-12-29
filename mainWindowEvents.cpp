// -*- coding: utf-8 -*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#include "mainWindow.h"
#include <QMenu>
#include <QKeyEvent>
#include <QMimeData>
#include <QDir>

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::mousePressEvent(QMouseEvent *event)
{
// either button toggles menu
// donn, Oct 23, 2022 changed to right button only, press
// left button to cancel. Added pause
//
    if (event->button() == Qt::RightButton) { // }||
//        event->button()==Qt::LeftButton) {
        QMenu menu(this);
        QAction * pause = menu.addAction(_pause ? "Continue" : "Pause");
        QAction * displayname = menu.addAction("Display File Name");
        QAction * quit = menu.addAction("Exit Program");
        QAction *selectedAction = menu.exec(QCursor::pos());
        if (selectedAction == pause) {
            _pause = !_pause;
            if (!_pause) nextImage();
        }
        
        if(selectedAction == quit) {
            close();
        }  else if (selectedAction == displayname) {
        // toggle display file name display
            _label[0]->_displayFileName = !_label[0]->_displayFileName;
            _label[1]->_displayFileName = !_label[1]->_displayFileName;
//            _label[0]->update();
//            _label[1]->update();
        }
    }
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::keyPressEvent(QKeyEvent *event)
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

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        for (int i = 0; i < 2; ++i) {
            resizeLabel(_label[i]);
        }
    }
    return QMainWindow::event(event);
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (numDegrees.y() < 0) {
        nextImage();
    } else if (numDegrees.y() > 0) {
        prevImage();
    }
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MainWindow::dropEvent(QDropEvent *event)
{
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
