#ifndef SLIDESHOWWIDGET_H
#define SLIDESHOWWIDGET_H

#include <QWidget>
#include <QXmlStreamReader>

#include "slideshow.h"

class SlideShowWidget : public QWidget
{
    Q_OBJECT
public:
    SlideShowWidget(QWidget* parent = 0);
    ~SlideShowWidget();
public slots:
//void resetInputTimeout();
//void inputTimedout();

//protected:
//void changeEvent(QEvent *) override;

private:
SlideShow* slideShow;
QTimer* inputTimer;
bool loadConfig(QString path);
void parseSlideshow(QXmlStreamReader& reader);
};
#endif
