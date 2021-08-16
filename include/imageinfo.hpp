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
    explicit ImageInfo(const QImage &img, const QString &path, QWidget *parent = nullptr);

private:
    QLabel                  *imageSize;
    QLabel                  *fileSize;
    QLabel                  *extension;
    QLabel                  *lastModified;
    QPushButton             *okBtn;
    static constexpr double BYTES_IN_MEGABYTES{1048576};
};

#endif // IMAGEINFO_HPP