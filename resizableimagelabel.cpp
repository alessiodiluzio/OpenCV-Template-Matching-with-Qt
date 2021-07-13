#include "resizableimagelabel.h"

#include <QPixmap>
#include <QImage>
#include <QResizeEvent>

ResizableImageLabel::ResizableImageLabel(QWidget *parent, const Qt::WindowFlags &f) :
    QLabel(parent, f)
    , _content(nullptr)
{
    setMinimumSize(1, 1);
}

__attribute__((unused)) void ResizableImageLabel::resizeEvent(QResizeEvent *event) {
    int w = event->size().width();
    int h = event->size().height();
    resize(w, h);
    if (!_content)
        return;
    setContent(_content);
}

void ResizableImageLabel::setContent(QImage *content) {
    if (!content)
        return;
    _content = content;
    QPixmap scaledPixmap = QPixmap::fromImage(*_content);
    setPixmap(scaledPixmap.scaled(width(),height(), Qt::KeepAspectRatioByExpanding));
}

QImage *ResizableImageLabel::getContent() const {
    return _content;
}

