#ifndef QTTEMPLATEMATCHING_MAINWINDOW_H
#define QTTEMPLATEMATCHING_MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QDoubleValidator>

#include "matchingmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ResizableImageLabel;

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, QImage *source = nullptr, QImage *templ = nullptr,
                        int method = 0, double threshold = 0.0);

    ~MainWindow() override;
    static void showImageDialog(QImage *image);


private:
    Ui::MainWindow *ui;

    ResizableImageLabel *_labelResizableSource;
    ResizableImageLabel *_labelResizableTemplate;
    ResizableImageLabel *_labelResizableResult;

    QDoubleValidator _qDoubleValidator;
    MatchingManager _matchingManager;

    void loadImage(const char* dialogTitle, ResizableImageLabel *dest);

    void drawResultLight(const QColor& color);

private slots:
    void loadSourceImage();
    void loadTemplateImage();
    void showSourceImage();
    void showTemplateImage();
    void showResultImage();
    void matchImage();
    void saveResultImage();
    void drawResult(double optimalVal, bool passed);
    void updateThresholdLabel(const QString &text);
};


#endif //QTTEMPLATEMATCHING_MAINWINDOW_H
