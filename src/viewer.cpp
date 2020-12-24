#include "viewer.hpp"
#include "ui_viewer.h"

Viewer::Viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Viewer)
{
    ui->setupUi(this);
    connect(ui->openImage,&QPushButton::clicked,this,&Viewer::onOpen);
    connect(ui->plus,&QPushButton::clicked,this,&Viewer::onZoomPlus);
    connect(ui->minus,&QPushButton::clicked,this,&Viewer::onZoomMinus);
    connect(ui->reset,&QPushButton::clicked,this,&Viewer::onReset);
    connect(ui->quit,&QPushButton::clicked,this,&QApplication::quit);
    connect(ui->nextImage,&QPushButton::clicked,this,&Viewer::onNext);
}

void Viewer::onOpen()
{
    imageName = QFileDialog::getOpenFileName(this);
    if(imageName.isEmpty())
    {
        QMessageBox::warning(this,"Image","Entrer un nom valide");
    }
    else
    {
        QImageReader reader(imageName);
        reader.setAutoTransform(true);
        img = reader.read();
        if (img.isNull())
        {
            QMessageBox::critical(this,"Image","Erreur lors de l'ouverture de l'image");
        }
        pixmap = QPixmap::fromImage(img);
        height = pixmap.height();
        width  = pixmap.width();
        ui->imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
        ui->imageLabel->setScaledContents(true);
        ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        ui->imageName->setText(imageName);
    }
    QStringList partsOfPath = imageName.split("/");
    QString directoryPath;
    for (auto i = 0;i < partsOfPath.length()-1;i++)
    {
        directoryPath.append((partsOfPath.at(i))+"/");
    }
    // Creating a QDir to store the current directory
    imageDirectory.setPath(directoryPath);
}

void Viewer::onZoomPlus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        height += 20;
        width  += 20;
        ui->imageLabel->setScaledContents(false);
        ui->imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
    }
}

void Viewer::onZoomMinus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        height -= 20;
        width  -= 20;
        ui->imageLabel->setScaledContents(false);
        ui->imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
    }
}

void Viewer::onReset()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        height = pixmap.height();
        width  = pixmap.width();
        ui->imageLabel->setScaledContents(true);
        ui->imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
    }
}

void Viewer::onNext()
{
    nbNext++;
    QDirIterator imgDirIterator{imageDirectory};
    if(imgDirIterator.hasNext())
    {
        auto i = 0;
        QString tmpImageName;
        while(imgDirIterator.hasNext() && (i != nbNext))
        {
            tmpImageName = imgDirIterator.next();
            i++;
        }
        QImageReader reader(tmpImageName);
        reader.setAutoTransform(true);
        img = reader.read();
        imageName = tmpImageName;
        QMessageBox::information(0,"test",tmpImageName);
        if (img.isNull())
        {
            QMessageBox::critical(this,"Image","Erreur lors de l'ouverture de l'image");
        }
        pixmap = QPixmap::fromImage(img);
        height = pixmap.height();
        width  = pixmap.width();
        ui->imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
        ui->imageLabel->setScaledContents(true);
        ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        ui->imageName->setText(imageName);
    }
}

Viewer::~Viewer()
{
    delete ui;
}

