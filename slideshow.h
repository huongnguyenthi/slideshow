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

#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QBasicTimer>
#include <QWidget>
#include <QThread>

class SlideShowPrivate : public QObject
{
    Q_OBJECT
public:
    SlideShowPrivate();

    int currentSlide;
    int slideInterval;
    QBasicTimer interSlideTimer;
    QStringList imagePaths;

    void showNextSlide();
    void showImage(QPaintEvent *event, QSize size, QWidget *slideshow);
    void clearImages();
    void startShow();
    void stopShow();

};


class SlideShow : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int slideInterval READ slideInterval WRITE setSlideInterval)

public:
    SlideShow(QWidget* parent = 0);
    ~SlideShow();
    void addImage(QString filename);
    void addImageDir(QString dirName);

    QThread slideShowThread;

    int slideInterval();
    void setSlideInterval(int val);
    void startShow();
    void stopShow();
    void clearImages();
    void printMessage();
signals:
    void inputReceived();
    void startSlideShow();
    void stopSlideShow();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void timerEvent(QTimerEvent* event);
    void showEvent(QShowEvent * event );


private:
    SlideShowPrivate* d;
};

#endif
