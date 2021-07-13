#include <QApplication>
#include <QPushButton>
#include <QMap>
#include <QDebug>

#include <iostream>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMap<QString, QString> _arguments;

    bool hasSource = false;
    bool hasTemplate = false;
    bool hasGUI = true;


    QString message = "Usage ./QtTemplateMatching [-help] [-nogui] [-src=source_image] [-templ=template_image] [-method=match_method]"
                      " [-threshold=match_threshold]\n"
                      "Options:\n"
                      "\t-help shows this message and exits.\n"
                      "\t-nogui (only valid when -src, -templ are specified) "
                      "run Template Matching without selection interface.\n"
                      "\t-src=source_image load source image from source_image path.\n"
                      "\t-templ=template_image load template image from template_image path.\n"
                      "\t-method=match_method set matching mode to match_method:\n"
                      "\t\t0) SQ_DIFF\n"
                      "\t\t1) SQ_DIFF_NORMED\n"
                      "\t\t2) CC_CORR\n"
                      "\t\t3) CC_CORR_NORMED\n"
                      "\t\t4) CC_COEFF_NORMED\n"
                      "\t\t5) CC_COEFF_NORMED\n"
                      "\t-threshold=match_threshold: threshold for matching algorithm.";


    QString source = "";
    QString templ = "";

    int method = 0;
    double threshold = 0.0;

    QImage *sourceImage = nullptr;
    QImage *templImage = nullptr;


    argc--;
    argv++;

    for (int i = 0; i < argc; ++i){
        QString arg = QString(argv[i]);
        if (arg == "-nogui") {
            hasGUI = false;
            continue;
        }
        else if (arg == "-help") {
            std::cout << message.toStdString() << std::endl;
            return 0;
        }
        QStringList tuple = arg.split("=");
        if (tuple.length() == 2)
            _arguments.insert(tuple[0], tuple[1]);

    }

    if (_arguments.contains("-src")){
        hasSource = true;
        source = _arguments.value("-src");
    }
    if (_arguments.contains("-templ")){
        hasTemplate = true;
        templ = _arguments.value("-templ");
    }
    if (_arguments.contains("-method")){
        method = _arguments.value("-method").toInt();
        if (method > 5)
            method = 0;
    }
    if (_arguments.contains("-threshold")){
        threshold = _arguments.value("-threshold").toDouble();
        if (threshold < 0)
            threshold = 0.0;
    }
    if (hasSource) {
        sourceImage = new QImage();
        if (!sourceImage->load(source)) {
            std::cout << "Error loading source image." << std::endl;
            return 0;
        }
    }

    if (hasTemplate) {
        templImage = new QImage();
        if (!templImage->load(templ)) {
            std::cout << "Error loading template image." << std::endl;
            return 0;
        }
    }

    if (!hasGUI and hasSource and hasTemplate) {
        MatchingManager matchingManager;
        auto mode = static_cast<cv::TemplateMatchModes>(method);

        QImage *resultImage = matchingManager.match(mode, sourceImage, templImage, threshold);

        if (resultImage)
            MainWindow::showImageDialog(resultImage);
        else {
            std::cout << "Error on computing Template Matching" << std::endl;
            return 0;
        }

    }

    else if (hasGUI) {
        auto *mainWindow = new MainWindow(nullptr, sourceImage, templImage, method, threshold);
        mainWindow->show();
    }

    else {
        std::cout << message.toStdString() << std::endl;
        return 0;
    }
    return QApplication::exec();
}
