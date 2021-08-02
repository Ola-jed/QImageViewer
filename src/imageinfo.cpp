#include "imageinfo.hpp"

ImageInfo::ImageInfo(const QImage &img, const QString &path,QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QFileInfo{path}.fileName());
    const QFileInfo fileInfo{path};
    imageSize    = new QLabel(QStringLiteral("Image size : %1 * %2").arg(img.width()).arg(img.height()), this);
    fileSize     = new QLabel(QStringLiteral("File size : %1 MB").arg(static_cast<double>(fileInfo.size()) / BYTES_IN_MEGABYTES), this);
    extension    = new QLabel(QStringLiteral("Extension : %1 ").arg(fileInfo.suffix()), this);
    lastModified = new QLabel(QStringLiteral("Last modification : %1 ").arg(fileInfo.lastModified().toString()));
    okBtn        = new QPushButton("Ok", this);
    auto lay     = new QVBoxLayout(this);
    lay->addWidget(imageSize);
    lay->addWidget(fileSize);
    lay->addWidget(extension);
    lay->addWidget(lastModified);
    lay->addWidget(okBtn);
    connect(okBtn, &QPushButton::clicked, [this] {
        close();
    });
}