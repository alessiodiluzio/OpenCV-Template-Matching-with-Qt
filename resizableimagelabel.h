#ifndef QTTEMPLATEMATCHING_RESIZABLEIMAGELABEL_H
#define QTTEMPLATEMATCHING_RESIZABLEIMAGELABEL_H


#include <QLabel>

class ResizableImageLabel: public QLabel {

public:
    explicit ResizableImageLabel(QWidget *parent, const Qt::WindowFlags &f = Qt::WindowFlags());

private:
    __attribute__((unused)) QImage *_content;
public:
    QImage *getContent() const;

public:
    void setContent(QImage *content);

protected:
    __attribute__((unused)) void resizeEvent(QResizeEvent *event) override;

};


#endif //QTTEMPLATEMATCHING_RESIZABLEIMAGELABEL_H
