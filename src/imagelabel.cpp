#include "imagelabel.hpp"

ImageLabel::ImageLabel(QWidget *parent)
    : QLabel(parent)
{
    setMouseTracking(true);
}

/// Mouse event
/// \param event
void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event->x(),event->y());
}