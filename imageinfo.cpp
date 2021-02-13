#include "imageinfo.hpp"

ImageInfo::ImageInfo(QWidget *parent,const QImage &img,const QString &path) : QDialog(parent)
{
    setWindowTitle(QFileInfo{path}.fileName());
    imageSize        = new QLabel("Image size : " + QString::number(img.width())+" * "+QString::number(img.height()),this);
    fileSize         = new QLabel("File size : "+QString::number(static_cast<double>(QFileInfo{path}.size())/1048576)+" mb",this);
    extension        = new QLabel("Extension : "+QFileInfo{path}.suffix(),this);
    lastModified     = new QLabel("Last Modified : "+QFileInfo{path}.lastModified().toString());
    okBtn            = new QPushButton("Ok",this);
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addWidget(imageSize);
    lay->addWidget(fileSize);
    lay->addWidget(extension);
    lay->addWidget(lastModified);
    lay->addWidget(okBtn);
    connect(okBtn,&QPushButton::clicked,this,[this]()->void{
        close();
    });
}
