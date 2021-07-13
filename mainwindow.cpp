#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "resizableimagelabel.h"

#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <QPicture>
#include <QPainter>
#include <QMessageBox>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent, QImage *source, QImage *templ,
                       int method, double threshold) :
        QWidget(parent)
        , ui(new Ui::MainWindow)
        , _labelResizableSource(nullptr)
        , _labelResizableTemplate(nullptr)
        , _labelResizableResult(nullptr)
{
    ui->setupUi(this);
    QMap<int, QString> map = MatchingManager::getFromMatchingMethods();
    for (auto it = map.begin(); it != map.end(); ++it)
        ui->_comboBoxMethod->addItem(it.value(), it.key());



    _labelResizableSource  = new ResizableImageLabel(this);
    _labelResizableTemplate  = new ResizableImageLabel(this);
    _labelResizableResult  = new ResizableImageLabel(this);

    ui->_lineEditThreshold->setValidator(&_qDoubleValidator);
    _qDoubleValidator.setRange(0.0, 1.0);
    ui->_lineEditThreshold->setText("0.0");

    ui->_labelResultIcon->setAlignment(Qt::AlignCenter);

    drawResultLight(Qt::yellow);
    ui->_labelValue->setText("0.0");



    auto *sourceLayout = dynamic_cast<QVBoxLayout*>(ui->_frameSource->layout());
    sourceLayout->insertWidget(1, _labelResizableSource);

    auto *templateLayout = dynamic_cast<QVBoxLayout*>(ui->_frameTemplate->layout());
    templateLayout->insertWidget(1, _labelResizableTemplate);

    auto *resultLayout = dynamic_cast<QVBoxLayout*>(ui->_frameResult->layout());
    resultLayout->insertWidget(1, _labelResizableResult);

    if (source) {
        _labelResizableSource->setContent(source);
    }
    if (templ) {
        _labelResizableTemplate->setContent(templ);
    }

    ui->_lineEditThreshold->setText(QString::number(threshold));


    connect(ui->_buttonLoadSource, &QPushButton::clicked, this, &MainWindow::loadSourceImage);
    connect(ui->_buttonLoadTemplate, &QPushButton::clicked, this, &MainWindow::loadTemplateImage);

    connect(ui->_buttonShowSource, &QPushButton::clicked, this, &MainWindow::showSourceImage);
    connect(ui->_buttonShowTemplate, &QPushButton::clicked, this, &MainWindow::showTemplateImage);
    connect(ui->_buttonShowResult, &QPushButton::clicked, this, &MainWindow::showResultImage);


    connect(ui->_buttonMatch, &QPushButton::clicked, this, &MainWindow::matchImage);
    connect(ui->_buttonSave, &QPushButton::clicked, this, &MainWindow::saveResultImage);

    connect(&_matchingManager, &MatchingManager::resultingVal, this, &MainWindow::drawResult);
    // connect(ui->_comboBoxMethod, SIGNAL(currentTextChanged(QString&text)),
       //     this, SLOT(updateThresholdLabel(QString&text)));
    connect(ui->_comboBoxMethod, &QComboBox::currentTextChanged,
            this, &MainWindow::updateThresholdLabel);

    ui->_comboBoxMethod->setCurrentIndex(method);


}

MainWindow::~MainWindow() {

    delete ui;

    delete _labelResizableResult;
    delete _labelResizableTemplate;
    delete _labelResizableSource;
}

void MainWindow::loadSourceImage() {
    loadImage("Select Source Image", _labelResizableSource);
}

void MainWindow::loadTemplateImage() {
    loadImage("Select Template Image", _labelResizableTemplate);

}

void MainWindow::loadImage(const char* dialogTitle, ResizableImageLabel *dest) {
    QString fileName = QFileDialog::getOpenFileName
            (this, tr(dialogTitle), tr("Images (*.png *.xpm *.jpg)"));
    auto *image = new QImage();
    if (!fileName.isEmpty() and fileName != "") {
        if (image->load(fileName)) {
            dest->setContent(image);
        }
    }
}

void MainWindow::matchImage() {

    QImage *source = _labelResizableSource->getContent();
    QImage *templ = _labelResizableTemplate->getContent();

    if (!source or !templ) {
        QMessageBox msgBox;
        msgBox.setText("Load a source and a template image before attempting to match.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (source->size().width() < templ->size().width() or source->size().height() < templ->size().height()){
        QMessageBox msgBox;
        msgBox.setText("Template image must be smaller than source.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    cv::TemplateMatchModes mode = static_cast<cv::TemplateMatchModes>(ui->_comboBoxMethod->currentData().value<int>());
    QImage *resultImage = _matchingManager.match(mode, source, templ, ui->_lineEditThreshold->text().toDouble());
    _labelResizableResult->setContent(resultImage);
}

void MainWindow::showImageDialog(QImage *image) {
    if (!image)
        return;

    auto *imageDialog = new QWidget();
    imageDialog->setBaseSize(800, 600);

    auto *imageLabel = new ResizableImageLabel(imageDialog);
    imageLabel->resize(800, 600);
    imageLabel->setContent(image);

    auto *imageLayout = new QVBoxLayout();
    imageLayout->insertWidget(0, imageLabel);
    imageDialog->setLayout(imageLayout);
    imageDialog->show();
}

void MainWindow::showSourceImage() {
    showImageDialog(_labelResizableSource->getContent());
}

void MainWindow::showTemplateImage() {
    showImageDialog(_labelResizableTemplate->getContent());
}

void MainWindow::showResultImage() {
    showImageDialog(_labelResizableResult->getContent());
}

void MainWindow::saveResultImage() {
    QImage *savedImage = _labelResizableResult->getContent();
    if (!savedImage)
        return;
    QString fileName = QFileDialog::getSaveFileName(this, "Save Result Image",
                                                    tr("result.png"));
    if (fileName.isEmpty() or fileName == "")
        return;
    savedImage->save(fileName);
}

void MainWindow::drawResult(double optimalVal, bool passed) {
    QColor resultColor = (passed) ? Qt::green : Qt::red;
    drawResultLight(resultColor);

    ui->_labelValue->setText(QString::number(optimalVal));
}

void MainWindow::drawResultLight(const QColor& color) {
    QPicture picture;
    QPainter painter(&picture);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);


    painter.setBackground(Qt::black);

    QPoint labelCenter(ui->_labelResultIcon->width()/2, ui->_labelResultIcon->height()/2);

    int radius = (ui->_labelResultIcon->width() < ui->_labelResultIcon->height()) ? ui->_labelResultIcon->width()/2 :
                 ui->_labelResultIcon->height()/2;

    painter.drawEllipse(labelCenter, radius , radius);

    ui->_labelResultIcon->setPicture(picture);
}

void MainWindow::updateThresholdLabel(const QString &text) {
    cv::TemplateMatchModes mode = static_cast<cv::TemplateMatchModes>(ui->_comboBoxMethod->currentData().toInt());
    QString labelText = "Max Threshold";
    if (mode == cv::TM_SQDIFF || mode == cv::TM_SQDIFF_NORMED)
        labelText = "Min Threshold";
    ui->_labelThreshold->setText(labelText);
}


