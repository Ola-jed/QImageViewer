#include "imageviewer.hpp"

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent)
{
    openImage      = new QPushButton("Open",this);
    quit           = new QPushButton("Quit",this);
    plus           = new QPushButton("+",this);
    minus          = new QPushButton("-",this);
    rotateDirect   = new QPushButton("⤴",this);
    rotateIndirect = new QPushButton("⤵",this);
    reset          = new QPushButton("Reset",this);
    diapoButton    = new QPushButton("Diaporama",this);
    imageLabel     = new QLabel(this);
    previousImage  = new QPushButton("<",this);
    nextImage      = new QPushButton(">",this);
    QVBoxLayout *menuVBox = new QVBoxLayout(this);
    menuVBox->addWidget(openImage);
    menuVBox->addWidget(quit);
    menuVBox->addWidget(plus);
    menuVBox->addWidget(minus);
    menuVBox->addWidget(rotateIndirect);
    menuVBox->addWidget(rotateDirect);
    menuVBox->addWidget(reset);
    menuVBox->addWidget(diapoButton);
    menuVBox->addStretch();
    menuVBox->setSpacing(0);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addLayout(menuVBox,2);
    layout->addWidget(previousImage,1);
    layout->addWidget(imageLabel,16);
    layout->addWidget(nextImage,1);
    auto central = new QWidget();
    central->setLayout(layout);
    setCentralWidget(central);
    setStyleSheet("QPushButton{background-color: rgb(28, 49, 80);color:#fff;}QLabel{color:#27fff8;}");
    previousImage->setStyleSheet("background-color:#335958");
    nextImage->setStyleSheet("background-color:#335958");
    connect(openImage,&QPushButton::clicked,this,&ImageViewer::onOpen);
    connect(plus,&QPushButton::clicked,this,&ImageViewer::onZoomPlus);
    connect(minus,&QPushButton::clicked,this,&ImageViewer::onZoomMinus);
    connect(reset,&QPushButton::clicked,this,&ImageViewer::onReset);
    connect(quit,&QPushButton::clicked,this,&QApplication::quit);
    connect(nextImage,&QPushButton::clicked,this,&ImageViewer::onNext);
    connect(previousImage,&QPushButton::clicked,this,&ImageViewer::onPrevious);
    connect(rotateDirect,&QPushButton::clicked,this,&ImageViewer::onRotateDirect);
    connect(rotateIndirect,&QPushButton::clicked,this,&ImageViewer::onRotateIndirect);
    connect(diapoButton,&QPushButton::clicked,this,&ImageViewer::onDiapo);
}


void ImageViewer::onOpen()
{
    imageName = QFileDialog::getOpenFileName(this);
    readImage(imageName);
    QStringList partsOfPath = imageName.split("/");
    QString directoryPath;
    for (auto i = 0;i < partsOfPath.length()-1;i++)
    {
        directoryPath.append((partsOfPath.at(i))+"/");
    }
    // Creating a QDir to store the current directory
    imageDirectory.setPath(directoryPath);
    setWindowTitle(imageName);
}

void ImageViewer::onZoomPlus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        height += 20;
        width  += 20;
        imageLabel->setScaledContents(false);
        imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
    }
}

void ImageViewer::onZoomMinus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        height -= 20;
        width  -= 20;
        imageLabel->setScaledContents(false);
        imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
    }
}

void ImageViewer::onReset()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        height = pixmap.height();
        width  = pixmap.width();
        imageLabel->setScaledContents(true);
        imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
    }
}

void ImageViewer::onNext()
{
    if(imageName.isEmpty())
        return;
    nbNext++;
    QDirIterator imgDirIterator{imageDirectory};
    if(imgDirIterator.hasNext())
    {
        auto i = 0;
        QString tmpImageName = " ";
        while(imgDirIterator.hasNext() && (i != nbNext))
        {
            previousImages.push(tmpImageName);
            tmpImageName = imgDirIterator.next();
            i++;
        }
        // We will check if the image is in the folder of the first image
        if(QFileInfo(tmpImageName).isFile())
        {
            imageName = tmpImageName;
            readImage(tmpImageName);
        }
    }
}

void ImageViewer::onPrevious()
{
    if(previousImages.length() > 2)
    {
        nbNext--;
        auto tempName {previousImages.pop()};
        readImage(tempName);
        imageName = tempName;
    }
}

void ImageViewer::onRotateDirect()
{
    angleRotation -= 90;
    readImageWithRotation(imageName,angleRotation);
}

void ImageViewer::onRotateIndirect()
{
    angleRotation += 90;
    readImageWithRotation(imageName,angleRotation);
}

void ImageViewer::onDiapo()
{
    QThread::sleep(3);
    QString tmpImageName = " ";
    QDirIterator imgDirIterator{imageDirectory};
    do
    {
        nbNext++;
        if(imgDirIterator.hasNext())
        {
            auto i = 0;
            while(imgDirIterator.hasNext() && (i != nbNext))
            {
                previousImages.push(tmpImageName);
                tmpImageName = imgDirIterator.next();
                i++;
            }
            if(QFileInfo(tmpImageName).isFile())
            {
                imageName = tmpImageName;
                readImage(tmpImageName);
            }
        }
    }while(QFileInfo(tmpImageName).isFile() && (imgDirIterator.hasNext()));
}

void ImageViewer::readImage(const QString &name)
{
    if(name.isEmpty())
    {
        QMessageBox::warning(this,"Image","Entrer un nom valide");
    }
    else
    {
        QImageReader reader(name);
        reader.setAutoTransform(true);
        img = reader.read();
        if (img.isNull())
        {
            QMessageBox::critical(this,"Image","Erreur lors de l'ouverture de l'image");
        }
        pixmap = QPixmap::fromImage(img);
        height = pixmap.height();
        width  = pixmap.width();
        imageLabel->setPixmap((QPixmap::fromImage(img)).scaled(height,width));
        imageLabel->setScaledContents(true);
        imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        setWindowTitle(name);
    }
}

void ImageViewer::readImageWithRotation(const QString &name,qreal angle)
{
    if(!name.isEmpty())
    {
        QImageReader reader(name);
        reader.setAutoTransform(true);
        img = reader.read();
        if (img.isNull())
        {
            QMessageBox::critical(this,"Image","Erreur lors de l'ouverture de l'image");
        }
        QTransform transformation {};
        transformation = transformation.rotate(angle);
        pixmap = QPixmap::fromImage(img);
        height = pixmap.height();
        width  = pixmap.width();
        imageLabel->setPixmap((QPixmap::fromImage(img)).transformed(transformation).scaled(height,width));
        imageLabel->setScaledContents(true);
        imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    }
}

ImageViewer::~ImageViewer()
{}
