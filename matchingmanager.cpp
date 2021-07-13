#include "matchingmanager.h"
#include "opencv2/highgui.hpp"

#include <QDebug>
#include <QPainter>


QMap<int, QString> MatchingManager::getFromMatchingMethods() {
    QMap<int, QString> matchingMethodsMap;

    matchingMethodsMap.insert(cv::TemplateMatchModes::TM_SQDIFF, "TM_SQDIFF");
    matchingMethodsMap.insert(cv::TemplateMatchModes::TM_SQDIFF_NORMED, "TM_SQDIFF_NORMED");
    matchingMethodsMap.insert(cv::TemplateMatchModes::TM_CCORR, "TM_CCORR");
    matchingMethodsMap.insert(cv::TemplateMatchModes::TM_CCORR_NORMED, "TM_CCORR_NORMED");
    matchingMethodsMap.insert(cv::TemplateMatchModes::TM_CCOEFF, "TM_CCOEFF");
    matchingMethodsMap.insert(cv::TemplateMatchModes::TM_CCOEFF_NORMED, "TM_CCOEFF_NORMED");

    return matchingMethodsMap;
}

QImage* MatchingManager::match(cv::TemplateMatchModes mode, QImage *sourceImage, QImage *templateImage, double threshold) {

    cv::Mat cvSourceImage = cv::Mat(sourceImage->height(), sourceImage->width(),
                                    CV_8UC4, sourceImage->bits(), sourceImage->bytesPerLine()).clone();
    cv::Mat cvTemplateImage = cv::Mat(templateImage->height(), templateImage->width(),
                                      CV_8UC4, templateImage->bits(), templateImage->bytesPerLine()).clone();
    cv::Mat resultImage;
    cv::Mat result;

    bool passed = false;

    cvSourceImage.copyTo( resultImage );

    int result_cols = cvSourceImage.cols - cvTemplateImage.cols + 1;
    int result_rows = cvSourceImage.rows - cvTemplateImage.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );


    matchTemplate( cvSourceImage, cvTemplateImage, result, mode);

    //normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; double optimalVal; cv::Point minLoc; cv::Point maxLoc;

    cv::Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if ( mode  == cv::TemplateMatchModes::TM_SQDIFF || mode == cv::TemplateMatchModes::TM_SQDIFF_NORMED ) {
        matchLoc = minLoc;
        if (minVal < threshold) {
            passed = true;
        }
        optimalVal = minVal;

    }
    else {
        matchLoc = maxLoc;
        if (maxVal > threshold) {
            passed = true;
        }
        optimalVal = maxVal;
    }

    emit resultingVal(optimalVal, passed);

    QImage *qResultImage = matToQImage(resultImage, sourceImage->format());

    if (passed) {
        QPainter qPainter(qResultImage);
        qPainter.setBrush(Qt::NoBrush);
        QPen pen;
        pen.setWidth(6);
        pen.setColor(Qt::black);
        qPainter.setPen(pen);
        qPainter.drawRect(matchLoc.x, matchLoc.y, cvTemplateImage.cols, cvTemplateImage.rows);
    }

    return  qResultImage;
}

QImage* MatchingManager::matToQImage(cv::Mat const &mat, QImage::Format format)
{
    return new QImage((uchar*) mat.data, mat.cols, mat.rows, mat.step, format);
}