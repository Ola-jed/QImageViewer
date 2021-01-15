#include "imageviewer.hpp"

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent)
{
    openImage      = new QPushButton(QIcon("assets/open.ico"),"",this);
    quit           = new QPushButton(QIcon("assets/quit.ico"),"",this);
    plus           = new QPushButton(QIcon("assets/plus.ico"),"",this);
    minus          = new QPushButton(QIcon("assets/minus.ico"),"",this);
    rotateDirect   = new QPushButton(QIcon("assets/direct.ico"),"",this);
    rotateIndirect = new QPushButton(QIcon("assets/indirect.ico"),"",this);
    reset          = new QPushButton(QIcon("assets/reset.ico"),"",this);
    diapoButton    = new QPushButton(QIcon("assets/diaporama.ico"),"",this);
    previousImage  = new QPushButton(QIcon("assets/previous.ico"),"",this);
    nextImage      = new QPushButton(QIcon("assets/next.ico"),"",this);
    randomImage    = new QPushButton(QIcon("assets/random.ico"),"",this);

    imageLabel     = new QLabel(this);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QVBoxLayout *menuVBox = new QVBoxLayout();
    menuVBox->addWidget(openImage);
    menuVBox->addWidget(quit);
    menuVBox->addWidget(plus);
    menuVBox->addWidget(minus);
    menuVBox->addWidget(rotateIndirect);
    menuVBox->addWidget(rotateDirect);
    menuVBox->addWidget(reset);
    menuVBox->addWidget(diapoButton);
    menuVBox->addWidget(randomImage);
    menuVBox->addStretch();
    menuVBox->setSpacing(0);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addLayout(menuVBox,1);
    layout->addWidget(previousImage,1);
    layout->addWidget(imageLabel,17);
    layout->addWidget(nextImage,1);
    auto central = new QWidget();
    central->setLayout(layout);
    setCentralWidget(central);

    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon("assets/icon.ico"));
    setMinimumSize(350,350);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
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
    connect(randomImage,&QPushButton::clicked,this,&ImageViewer::onRandom);
}

// Opening a picture
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

// Zoom
void ImageViewer::onZoomPlus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(1.25);
    }
}

void ImageViewer::onZoomMinus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(0.8);
    }
}

void ImageViewer::onReset()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        zoomFactor = 1;
        readImage(imageName);
    }
}

void ImageViewer::scaleImage(double factor)
{
    zoomFactor *= factor;
    imageLabel->resize(zoomFactor*imageLabel->pixmap(Qt::ReturnByValue).size());
}

// Moving in the image directory
void ImageViewer::onNext()
{
    if(imageName.isEmpty())
        return;
    QDirIterator imgDirIterator{imageDirectory};
    if(imgDirIterator.hasNext())
    {
        nbNext++;
        int i = 0;
        QString tmpImageName = imageName;
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
    if((previousImages.length() > 0) && (nbNext > 0))
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
    QString tmpImageName;
    QDirIterator imgDirIterator{imageDirectory};
    QList<QString> imageList;
    while((imgDirIterator.hasNext()) && (QFileInfo(tmpImageName = imgDirIterator.next())).isFile())
    {
        imageList.push_back(tmpImageName);
    }
    foreach(auto tmp,imageList)
    {
        QTimer timer;
        QEventLoop loop; // Event loop to read the images during a time.
        connect(&timer,&QTimer::timeout,&loop,&QEventLoop::quit);
        timer.start(TIME_TO_WAIT);
        readImage(tmp);
        loop.exec();
    }
}

void ImageViewer::onRandom()
{
    // We will get all the elements of the current directory.
    QDirIterator imgIterator{imageDirectory};
    QList<QString> otherImages;
    QString tmpName;
    while((imgIterator.hasNext()) && (QFileInfo(tmpName = imgIterator.next()).isFile()))
    {
        otherImages.push_back(tmpName);
    }
    // Get a random image.
    auto randomImage = otherImages[QRandomGenerator::global()->bounded(0,otherImages.size())];
    imageName        = randomImage;
    readImage(randomImage);
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
