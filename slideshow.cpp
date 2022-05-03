/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QList>
#include <QImage>
#include <QDir>
#include <QPainter>
#include <QPaintEvent>
#include <QThread>
#include <QDebug>

#include "slideshow.h"



SlideShowPrivate::SlideShowPrivate()
{
    currentSlide = 0;
    slideInterval = 10000; // Default to 10 sec interval
}


void SlideShowPrivate::showNextSlide()
{
    currentSlide++;
    if (currentSlide >= imagePaths.size())
      currentSlide = 0;
}

void SlideShowPrivate::showImage(QPaintEvent *event, QSize size, QWidget *slideshow)
{
     QPainter painter(slideshow);
    painter.setRenderHint(QPainter::Antialiasing, false);
    if (imagePaths.size() > 0) {
        QPixmap slide = QPixmap(imagePaths[currentSlide]);
        QSize slideSize = slide.size();
        QSize scaledSize = QSize(qMin(slideSize.width(), size.width()),
            qMin(slideSize.height(), size.height()));
        if (slideSize != scaledSize)
            slide = slide.scaled(scaledSize, Qt::KeepAspectRatio);

        QRect pixmapRect(qMax( (size.width() - slide.width())/2, 0),
                         qMax( (size.height() - slide.height())/2, 0),
                         slide.width(),
                         slide.height());

        if (pixmapRect.top() > 0) {
            // Fill in top & bottom rectangles:
            painter.fillRect(0, 0, size.width(), pixmapRect.top(), Qt::black);
            painter.fillRect(0, pixmapRect.bottom(), size.width(), size.height(), Qt::black);
        }

        if (pixmapRect.left() > 0) {
            // Fill in left & right rectangles:
            painter.fillRect(0, 0, pixmapRect.left(), size.height(), Qt::black);
            painter.fillRect(pixmapRect.right(), 0, size.width(), size.height(), Qt::black);
        }

        painter.drawPixmap(pixmapRect, slide);

    } else
        painter.fillRect(event->rect(), Qt::black);
}

void SlideShowPrivate::clearImages()
{
    imagePaths.clear();
}


void SlideShowPrivate::startShow()
{
    interSlideTimer.start(slideInterval, this);
    showNextSlide();
}


void SlideShowPrivate::stopShow()
{
    interSlideTimer.stop();
}


SlideShow::SlideShow(QWidget* parent) :
    QWidget(parent)

{
    d = new SlideShowPrivate;
    slideShowThread.setObjectName("SlideShowThread");
    d ->moveToThread(&slideShowThread);
    d->setObjectName("SlideShowThread");


    QObject::connect(&slideShowThread, &QThread::finished, d, &QObject::deleteLater);


    setAttribute(Qt::WA_StaticContents, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);

    setMouseTracking(true);
}


SlideShow::~SlideShow()
{
    delete d;
}


void SlideShow::addImageDir(QString dirName)
{
    QDir dir(dirName);
    qDebug() << "add Image Dir " << dirName;
    QStringList fileNames = dir.entryList(QDir::Files | QDir::Readable, QDir::Name);
    qDebug() << "addImageDir: get filenames ";
    for (int i=0; i<fileNames.count(); i++)
    {
      qDebug() << "addImageDir: number of file names " << fileNames.count();
        d->imagePaths << dir.absoluteFilePath(fileNames[i]);
        qDebug() << "addImageDir: imagePath" << dir.absoluteFilePath(fileNames[i]);
     }
}

void SlideShow::addImage(QString filename)
{
    qDebug() << "addImage: filename" << filename;
    d->imagePaths << filename;
}



int SlideShow::slideInterval()
{
    return d->slideInterval;
}

void SlideShow::setSlideInterval(int val)
{
    d->slideInterval = val;
}


void SlideShow::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    d->showNextSlide();
    update();
}

void SlideShow::startShow()
{
    emit startSlideShow();
}

void SlideShow::stopShow()
{
    d->stopShow();
}

void SlideShow::clearImages()
{
    d->clearImages();
}

void SlideShow::paintEvent(QPaintEvent *event)
{
    QSize size = this->size();
    d->showImage(event, size, this);
/*
    painter.setRenderHint(QPainter::Antialiasing, false);
    qDebug()<< "paint Event: slide index" << d->currentSlide;I
    qDebug()<< "paint Event: current image " << d->imagePaths[d->currentSlide];
    if (d->imagePaths.size() > 0) {
        QPixmap slide = QPixmap(d->imagePaths[d->currentSlide]);
        QSize slideSize = slide.size();
        QSize scaledSize = QSize(qMin(slideSize.width(), size().width()),
            qMin(slideSize.height(), size().height()));
        if (slideSize != scaledSize)
            slide = slide.scaled(scaledSize, Qt::KeepAspectRatio);

        QRect pixmapRect(qMax( (size().width() - slide.width())/2, 0),
                         qMax( (size().height() - slide.height())/2, 0),
                         slide.width(),
                         slide.height());

        if (pixmapRect.top() > 0) {
            // Fill in top & bottom rectangles:
            painter.fillRect(0, 0, size().width(), pixmapRect.top(), Qt::black);
            painter.fillRect(0, pixmapRect.bottom(), size().width(), size().height(), Qt::black);
        }

        if (pixmapRect.left() > 0) {
            // Fill in left & right rectangles:
            painter.fillRect(0, 0, pixmapRect.left(), size().height(), Qt::black);
            painter.fillRect(pixmapRect.right(), 0, size().width(), size().height(), Qt::black);
        }

        painter.drawPixmap(pixmapRect, slide);

    } else
        painter.fillRect(event->rect(), Qt::black);
        */
}


void SlideShow::keyPressEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}


void SlideShow::mouseMoveEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}


void SlideShow::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}

void SlideShow::printMessage()
{
    qDebug()<< "Slideshow Thread start";
}

void SlideShow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit inputReceived();
}


void SlideShow::showEvent(QShowEvent * event )
{
    Q_UNUSED(event);
#ifndef QT_NO_CURSOR
    setCursor(Qt::BlankCursor);
#endif
}

