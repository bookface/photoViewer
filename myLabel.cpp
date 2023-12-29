// -*- coding: utf-8 -*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

#include <QLabel>
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QPainterPath>

#include "mainWindow.h"

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
MyLabel::MyLabel(QWidget *parent ):QLabel(parent)
{
    _font.setFamily("Times");
    _font.setPointSize(30);
    _font.setWeight(QFont::Bold);
    _font.setStyleStrategy(QFont::PreferAntialias);
    setAutoFillBackground(true);
    _effect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(_effect);
    _effect->setOpacity(1.0);
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MyLabel::setOpacity(qreal v)
{
    if (v >= 0) {
        _effect->setOpacity(v);
    }
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void MyLabel::paintEvent(QPaintEvent *)
{
    QPainter p(this);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    const QPixmap pmap = this->pixmap();
    p.drawPixmap(rect(), pmap);
#else // assume 5
    const QPixmap *pmap = this->pixmap();
    p.drawPixmap(rect(), *pmap);
#endif

// old large top of screen, file name display
#if 0
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
#endif
    
// new small, lower left file name display
    if (_displayFileName && _text.length()) {
//            p.setBrush(QColor(Qt::white));
        p.setBrush(QColor(Qt::black));
        auto len = _text.length() * 10; // 360
        QRectF rectangle(0, height()-30, len, 60.0);
        p.drawRect(rectangle);
        QFont font = p.font();
        font.setPointSize(15);
        p.setFont(font);
        QPen pen;
//            pen.setColor(Qt::black);
        pen.setColor(Qt::white);
        p.setPen(pen);
        p.drawText(0,height()-10,_text);
    }
}
