#ifndef QIMAGEVIEWER_IMAGELABEL_HPP
#define QIMAGEVIEWER_IMAGELABEL_HPP

#include <QDebug>
#include <QLabel>
#include <QMouseEvent>

class ImageLabel: public QLabel
{
    Q_OBJECT

    public:
        explicit ImageLabel(QWidget *parent = nullptr);

    protected:
        void mouseMoveEvent(QMouseEvent *) override;

    signals:
        void mouseMoved(int x,int y);
};

#endif //QIMAGEVIEWER_IMAGELABEL_HPP
