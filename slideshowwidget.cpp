#include <QXmlStreamReader>
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QHBoxLayout>
#include "slideshowwidget.h"

#define DEFAULT_INPUT_TIMEOUT 10000
#define SIZING_FACTOR_HEIGHT 6/10
#define SIZING_FACTOR_WIDTH 6/10

SlideShowWidget::SlideShowWidget(QWidget* parent)
{
    slideShow = new SlideShow();
    inputTimer = new QTimer();

    inputTimer->setSingleShot(true);
    inputTimer->setInterval(DEFAULT_INPUT_TIMEOUT);

    bool success;
    QString path = QCoreApplication::instance()->applicationDirPath() + "/config.xml";

    success = loadConfig(path);

    if (success) {
        inputTimer->start();
    } else {

        QMessageBox msgBox;
        msgBox.setText("Could not load config file.");
        msgBox.exec();
    }

    slideShow->setWindowTitle("slide show");
    slideShow->show();
}

SlideShowWidget::~SlideShowWidget()
{
    delete slideShow;

}
bool SlideShowWidget::loadConfig(QString configPath)
{
    QFile xmlFile(configPath);

    if (!xmlFile.exists() || (xmlFile.error() != QFile::NoError)) {
        qDebug() << "ERROR: Unable to open config file " << configPath;
        return false;
    }

    slideShow->clearImages();

    xmlFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&xmlFile);
    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement()) {
            if(reader.name() == "slideshow")
                parseSlideshow(reader);
        }
    }

    if (reader.hasError()) {
        qDebug() << QString("Error parsing %1 on line %2 column %3: \n%4")
                    .arg(configPath)
                    .arg(reader.lineNumber())
                    .arg(reader.columnNumber())
                    .arg(reader.errorString());
    }

   return true;
}


void SlideShowWidget::parseSlideshow(QXmlStreamReader& reader)
{
    QXmlStreamAttributes attrs = reader.attributes();

    QStringRef timeout = attrs.value("timeout");
    bool valid;
    if (!timeout.isEmpty()) {
        int t = timeout.toString().toInt(&valid);
        if (valid)
            inputTimer->setInterval(t);
    }

    QStringRef interval = attrs.value("interval");
    if (!interval.isEmpty()) {
        int i = interval.toString().toInt(&valid);
        if (valid)
            slideShow->setSlideInterval(i);
    }

    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            QXmlStreamAttributes attrs = reader.attributes();
            if (reader.name() == "imagedir") {
                QStringRef dir = attrs.value("dir");
                slideShow->addImageDir(dir.toString());
            } else if(reader.name() == "image") {
                QStringRef image = attrs.value("image");
                slideShow->addImage(image.toString());
            }
        } else if(reader.isEndElement() && reader.name() == "slideshow") {
            return;
        }
    }

}
