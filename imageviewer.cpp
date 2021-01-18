#include "imageviewer.hpp"

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent)
{
    buildComponents();
    buildMenu();
    setShortcuts();
    applyStyle();
    applyLayout();
    setAcceptDrops(true);
    connect(openImage,&QAction::triggered,this,&ImageViewer::onDialogOpen);
    connect(plus,&QAction::triggered,this,&ImageViewer::onZoomPlus);
    connect(minus,&QAction::triggered,this,&ImageViewer::onZoomMinus);
    connect(reset,&QAction::triggered,this,&ImageViewer::onReset);
    connect(quit,&QAction::triggered,this,&QApplication::quit);
    connect(rotateDirect,&QAction::triggered,this,&ImageViewer::onRotateDirect);
    connect(rotateIndirect,&QAction::triggered,this,&ImageViewer::onRotateIndirect);
    connect(diapoButton,&QAction::triggered,this,&ImageViewer::onDiapo);
    connect(randomImage,&QAction::triggered,this,&ImageViewer::onRandom);
    connect(diapoTime,&QAction::triggered,this,&ImageViewer::onDiapoTime);
    connect(nextImage,&QPushButton::clicked,this,&ImageViewer::onNext);
    connect(previousImage,&QPushButton::clicked,this,&ImageViewer::onPrevious);
}

// Components creation.
void ImageViewer::buildComponents()
{
    file           = new QMenu("File",this);
    zoom           = new QMenu("Zoom",this);
    rotation       = new QMenu("Rotation",this);
    advanced       = new QMenu("Advanced",this);
    openImage      = new QAction(QIcon("assets/open.ico"),"Open",this);
    quit           = new QAction(QIcon("assets/quit.ico"),"Quit",this);
    plus           = new QAction(QIcon("assets/plus.ico"),"Zoom In",this);
    minus          = new QAction(QIcon("assets/minus.ico"),"Zoom out",this);
    rotateDirect   = new QAction(QIcon("assets/direct.ico"),"Rotate direct",this);
    rotateIndirect = new QAction(QIcon("assets/indirect.ico"),"Rotate indirect",this);
    reset          = new QAction(QIcon("assets/reset.ico"),"Reset",this);
    diapoButton    = new QAction(QIcon("assets/diaporama.ico"),"Diaporama",this);
    diapoTime      = new QAction(QIcon("assets/timer.ico"),"Diaporama duration",this);
    randomImage    = new QAction(QIcon("assets/random.ico"),"Random",this);
    previousImage  = new QPushButton(QIcon("assets/previous.ico"),"");
    nextImage      = new QPushButton(QIcon("assets/next.ico"),"");
    imageLabel     = new QLabel(this);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    previousImage->setDisabled(true);
    nextImage->setDisabled(true);
}

// Menu building.
void ImageViewer::buildMenu()
{
    myMenu = new QMenuBar(this);
    file->addAction(openImage);
    file->addAction(quit);
    zoom->addAction(minus);
    zoom->addAction(plus);
    zoom->addAction(reset);
    rotation->addAction(rotateDirect);
    rotation->addAction(rotateIndirect);
    rotation->addAction(reset);
    advanced->addAction(diapoButton);
    advanced->addAction(diapoTime);
    advanced->addAction(randomImage);
    myMenu->addMenu(file);
    myMenu->addSeparator();
    myMenu->addMenu(zoom);
    myMenu->addSeparator();
    myMenu->addMenu(rotation);
    myMenu->addSeparator();
    myMenu->addMenu(advanced);
}

// Shortcuts.
void ImageViewer::setShortcuts()
{
    openImage->setShortcut(QKeySequence::Open);
    quit->setShortcut(QKeySequence::Quit);
    plus->setShortcut(QKeySequence::ZoomIn);
    minus->setShortcut(QKeySequence::ZoomOut);
    reset->setShortcuts({QKeySequence::Refresh,QKeySequence::Cancel});
}

// Stylesheet and window resizing.
void ImageViewer::applyStyle()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon("assets/icon.ico"));
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    setStyleSheet(STYLE);
}

void ImageViewer::applyLayout()
{
    QHBoxLayout *buttonLay = new QHBoxLayout();
    buttonLay->setAlignment(Qt::AlignHCenter);
    buttonLay->addWidget(previousImage);
    buttonLay->addWidget(nextImage);
    QVBoxLayout *appLayout = new QVBoxLayout();
    appLayout->setContentsMargins(0,0,0,15);
    appLayout->addWidget(myMenu,1);
    appLayout->addWidget(imageLabel,16);
    appLayout->addLayout(buttonLay,2);
    auto central = new QWidget(this);
    central->setLayout(appLayout);
    setCentralWidget(central);
}

// Dialog file to choos ethe image to open
void ImageViewer::onDialogOpen()
{
    imageName = QFileDialog::getOpenFileName(this);
    onOpen(imageName);
}

// Opening a picture
void ImageViewer::onOpen(const QString &fileImage)
{
    readImage(fileImage);
    nextImage->setDisabled(false);
    QStringList partsOfPath = fileImage.split("/");
    QString directoryPath;
    for (auto i = 0;i < partsOfPath.length()-1;i++)
    {
        directoryPath.append((partsOfPath.at(i))+"/");
    }
    // Creating a QDir to store the current directory
    imageDirectory.setPath(directoryPath);
    setWindowTitle(fileImage);
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
    if(imageName.isEmpty()) return;
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
            previousImage->setDisabled(false);
            imageName = tmpImageName;
            readImage(tmpImageName);
        }
    }
}

void ImageViewer::onPrevious()
{
    if((previousImages.length() > 0) && (nbNext >= 0))
    {
        nbNext--;
        auto tempName {previousImages.pop()};
        readImage(tempName);
        imageName = tempName;
        if(previousImages.length() == 0)
        {
            previousImage->setDisabled(true); // There is no previous image
        }
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

void ImageViewer::onDiapoTime()
{
    QMessageBox::information(this,"Diaporama duration","Choose the diaporama duration in seconds");
    QSpinBox *numberBox = new QSpinBox(nullptr);
    numberBox->setRange(1,30);
    numberBox->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    numberBox->setFixedSize(100,100);
    connect(numberBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&ImageViewer::changeDiapoTime);
    numberBox->show();
}

void ImageViewer::changeDiapoTime(int time)
{
    time         = std::abs(time);
    TIME_TO_WAIT = static_cast<long>(time);
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
        QMessageBox::warning(this,"Image","Enter a valid name");
    }
    else
    {
        QImageReader reader(name);
        reader.setAutoTransform(true);
        img = reader.read();
        if (img.isNull())
        {
            QMessageBox::critical(this,"Image","Cannot open the image");
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
            QMessageBox::critical(this,"Image","Cannot open the image");
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

// Drag event to open images.
void ImageViewer::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

// Drop event to open Images.
void ImageViewer::dropEvent(QDropEvent *event)
{
   const QMimeData* mimeData = event->mimeData();
   // Check for our needed mime type, here a file or a list of files
   if (mimeData->hasUrls())
   {
     QList<QUrl> urlList = mimeData->urls();
     // Extract the local paths of the files.
     // This code is only valid in linux because of the path . Needs to be adapted on windows
     imageName = urlList[0].toString().right(urlList[0].toString().length() - 7);
     onOpen(imageName);
   }
}

ImageViewer::~ImageViewer()
{}
