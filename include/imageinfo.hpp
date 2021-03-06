#ifndef IMAGEINFO_HPP
#define IMAGEINFO_HPP

#include <QImage>
#include <QLabel>
#include <QDialog>
#include <QObject>
#include <QDateTime>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class ImageInfo : public QDialog
{
    Q_OBJECT

    public:
        explicit ImageInfo(QWidget *parent = nullptr,const QImage &img = {},const QString &path = "");

    private:
        QLabel *imageSize;
        QLabel *fileSize;
        QLabel *extension;
        QLabel *lastModified;
        QPushButton *okBtn;
};

#endif // IMAGEINFO_HPP