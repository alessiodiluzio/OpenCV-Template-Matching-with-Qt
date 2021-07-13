#ifndef QTTEMPLATEMATCHING_MATCHINGMANAGER_H
#define QTTEMPLATEMATCHING_MATCHINGMANAGER_H

#include "opencv2/imgproc.hpp"

#include <QMap>
#include <QString>
#include <QImage>

class MatchingManager: public QObject {
    Q_OBJECT
public:
    MatchingManager() = default;

    static QMap<int, QString> getFromMatchingMethods();
    QImage* match(cv::TemplateMatchModes mode, QImage *sourceImage, QImage *templateImage, double threshold);

private:
    static QImage* matToQImage(cv::Mat const &mat, QImage::Format format);

signals:
    void resultingVal(double val, bool passed);


};


#endif //QTTEMPLATEMATCHING_MATCHINGMANAGER_H
