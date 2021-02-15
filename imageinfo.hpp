#ifndef IMAGEINFO_HPP
#define IMAGEINFO_HPP

#include <QDateTime>
#include <QDialog>
#include <QObject>
#include <QImage>
#include <QLabel>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
class ImageInfo : public QDialog
{
    Q_OBJECT
public:
    ImageInfo(QWidget *parent = nullptr,const QImage &img = {},const QString &path = "");
private:
    QLabel *imageSize;
    QLabel *fileSize;
    QLabel *extension;
    QLabel *lastModified;
    QPushButton *okBtn;
};

#endif // IMAGEINFO_HPP
