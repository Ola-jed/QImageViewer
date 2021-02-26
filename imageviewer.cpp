#include "imageviewer.hpp"

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    buildComponents();
    buildMenu();
    setShortcuts();
    applyStyle();
    applyLayout();
    setAcceptDrops(true);
    makeConnections();
    timeToWait = (imgViewerSettings.value("Time").toInt() > 1000) ? imgViewerSettings.value("Time").toInt() : 4000;
}

// Make the connections.
void ImageViewer::makeConnections()
{
    connect(nextImage,&QPushButton::clicked,this,&ImageViewer::onNext);
    connect(previousImage,&QPushButton::clicked,this,&ImageViewer::onPrevious);
    connect(openImage,&QAction::triggered,this,&ImageViewer::onDialogOpen);
    connect(plus,&QAction::triggered,this,&ImageViewer::onZoomPlus);
    connect(minus,&QAction::triggered,this,&ImageViewer::onZoomMinus);
    connect(reset,&QAction::triggered,this,&ImageViewer::onReset);
    connect(quit,&QAction::triggered,this,&QApplication::quit);
    connect(info,&QAction::triggered,this,&ImageViewer::showInfo);
    connect(rotateDirect,&QAction::triggered,this,&ImageViewer::onRotateDirect);
    connect(rotateIndirect,&QAction::triggered,this,&ImageViewer::onRotateIndirect);
    connect(diapoButton,&QAction::triggered,this,&ImageViewer::onDiapo);
    connect(randomImage,&QAction::triggered,this,&ImageViewer::onRandom);
    connect(diapoTime,&QAction::triggered,this,&ImageViewer::onDiapoTime);
}

// Components creation.
void ImageViewer::buildComponents()
{
    file           = new QMenu("File",this);
    zoom           = new QMenu("Zoom",this);
    rotation       = new QMenu("Rotation",this);
    advanced       = new QMenu("Advanced",this);
    openImage      = new QAction(QIcon(":assets/open.ico"),"Open",this);
    quit           = new QAction(QIcon(":assets/quit.ico"),"Quit",this);
    plus           = new QAction(QIcon(":assets/plus.ico"),"Zoom In",this);
    minus          = new QAction(QIcon(":assets/minus.ico"),"Zoom out",this);
    rotateDirect   = new QAction(QIcon(":assets/direct.ico"),"Rotate direct",this);
    rotateIndirect = new QAction(QIcon(":assets/indirect.ico"),"Rotate indirect",this);
    reset          = new QAction(QIcon(":assets/reset.ico"),"Reset",this);
    diapoButton    = new QAction(QIcon(":assets/diaporama.ico"),"Diaporama",this);
    diapoTime      = new QAction(QIcon(":assets/timer.ico"),"Diaporama duration",this);
    randomImage    = new QAction(QIcon(":assets/random.ico"),"Random",this);
    info           = new QAction(QIcon(":assets/info.ico"),"Info",this);
    previousImage  = new QPushButton(QIcon(":assets/previous.ico"),"");
    nextImage      = new QPushButton(QIcon(":assets/next.ico"),"");
    imageLabel     = new QLabel(this);
    positionBar    = new QStatusBar(this);
    imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
}

// Menu building.
void ImageViewer::buildMenu()
{
    myMenu = new QMenuBar();
    file->addAction(openImage);
    file->addAction(quit);
    file->addSeparator();
    file->addAction(info);
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
    setWindowIcon(QIcon(":assets/icon.ico"));
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    info->setDisabled(true);
    previousImage->setDisabled(true);
    nextImage->setDisabled(true);
    setStyleSheet(STYLE);
}

// Applying a layout to the mainwindow.
void ImageViewer::applyLayout()
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(myMenu);
    QHBoxLayout *buttonLay = new QHBoxLayout();
    buttonLay->setAlignment(Qt::AlignHCenter);
    buttonLay->addWidget(previousImage);
    buttonLay->addWidget(nextImage);
    QVBoxLayout *appLayout = new QVBoxLayout(this);
    appLayout->setContentsMargins(0,0,0,0);
    appLayout->addLayout(topLayout,1);
    appLayout->addWidget(imageLabel,17);
    appLayout->addLayout(buttonLay,1);
    appLayout->addWidget(positionBar,1);
}

// Dialog file to choose the image to open
void ImageViewer::onDialogOpen()
{
    currentImageName = QFileDialog::getOpenFileName(this);
    if(currentImageName.isEmpty())
    {
        QMessageBox::warning(this,"Image","Enter a valid name");
    }
    else
    {
        onOpen(currentImageName);
    }
}

// Opening a picture
void ImageViewer::onOpen(const QString &fileImage)
{
    // Storing the current directory
    imageDirectory.setPath(QFileInfo{fileImage}.absoluteDir().path()+"/");
    fillNextElements();
    readImage(fileImage);
    setWindowTitle(fileImage);
}

// Zoom
void ImageViewer::onZoomPlus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(1.18);
    }
}

void ImageViewer::onZoomMinus()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        scaleImage(0.8);
    }
}

// Reset the image scaling or the rotation
void ImageViewer::onReset()
{
    if(!QPixmap::fromImage(img).isNull())
    {
        zoomFactor    = 1;
        angleRotation = 0;
        readImage(currentImageName);
    }
}

// Scaling the image for the zoom.
void ImageViewer::scaleImage(double factor)
{
    zoomFactor *= factor;
    const QSize size {imageLabel->pixmap(Qt::ReturnByValue).size() * zoomFactor};
    imageLabel->resize(size);
}

// Fill the list of next images in the directory
void ImageViewer::fillNextElements()
{
    nextImages.clear();
    previousImages.clear();
    QDirIterator imageDirIt{imageDirectory};
    QString tmpImage;
    while(imageDirIt.hasNext() && (QFileInfo((tmpImage = imageDirIt.next())).isFile()))
    {
        if(tmpImage != currentImageName)
        {
            nextImages.push_back(tmpImage);
        }
    }
}

// Read an image in the imageLabel.
void ImageViewer::readImage(const QString &name)
{
    QImageReader reader(name);
    reader.setAutoTransform(true);
    img = reader.read();
    if (img.isNull())
    {
        QMessageBox::critical(this,"Image","Cannot open the image");
        return;
    }
    pixmap = QPixmap::fromImage(img);
    height = pixmap.height();
    width  = pixmap.width();
    info->setDisabled(false);
    nextImage->setDisabled((nextImages.size() <= 0));
    previousImage->setDisabled((previousImages.size() <= 0));
    imageLabel->setPixmap(pixmap);
    imageLabel->setScaledContents(true);
    currentImageName = name;
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setWindowTitle(name);
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
        currentImageName = name;
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}

// Read the following image in the directory.
void ImageViewer::onNext()
{
    if(nextImages.size() > 0)
    {
        const auto imgToRead{nextImages.takeFirst()};
        previousImages.push_front(currentImageName);
        readImage(imgToRead);
     }
}

// Read the previous image in the directory.
void ImageViewer::onPrevious()
{
    if(previousImages.size() > 0)
    {
        const auto imgToRead{previousImages.takeFirst()};
        nextImages.push_front(currentImageName);
        readImage(imgToRead);
    }
}

void ImageViewer::onRotateDirect()
{
    angleRotation -= 90;
    readImageWithRotation(currentImageName,angleRotation);
}

void ImageViewer::onRotateIndirect()
{
    angleRotation += 90;
    readImageWithRotation(currentImageName,angleRotation);
}

// Diaporama.
void ImageViewer::onDiapo()
{
    startDiapo();
    QString tmpImageName;
    QDirIterator imgDirIterator{imageDirectory};
    QList<QString> imageList;
    while((imgDirIterator.hasNext()) && (QFileInfo(tmpImageName = imgDirIterator.next())).isFile())
    {
        imageList.push_back(tmpImageName);
    }
    foreach(auto tmp,imageList)
    {
        if(!isRunningDiapo) break;
        QTimer timer;
        QEventLoop loop; // Event loop to read the images during a time.
        connect(&timer,&QTimer::timeout,&loop,&QEventLoop::quit);
        timer.start(timeToWait);
        readImage(tmp);
        loop.exec();
        if(!isRunningDiapo) break;
    }
    endDiapo();
}

// Changing the duration of the diaporama
void ImageViewer::onDiapoTime()
{
    QMessageBox::information(this,"Diaporama duration","Choose the diaporama duration in seconds");
    QSpinBox *numberBox = new QSpinBox();
    numberBox->setRange(1,30);
    numberBox->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    numberBox->setFixedSize(100,100);
    connect(numberBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&ImageViewer::changeDiapoTime);
    numberBox->show();
}

// Updating the time
void ImageViewer::changeDiapoTime(int time)
{
    time       = std::abs(time);
    timeToWait = time*1000;
    imgViewerSettings.setValue("Time",QString::number(timeToWait));
}

// Start the diaporama.
void ImageViewer::startDiapo()
{
    nextImage->setVisible(false);
    previousImage->setVisible(false);
    myMenu->setVisible(false);
    positionBar->setVisible(false);
    isRunningDiapo = true;
    setFullScreen(true);
}

// End the diaporama
void ImageViewer::endDiapo()
{
    nextImage->setVisible(true);
    previousImage->setVisible(true);
    myMenu->setVisible(true);
    positionBar->setVisible(true);
    isRunningDiapo = false;
    setFullScreen(false);
}

void ImageViewer::onRandom()
{
    auto const randomPrevious{previousImages.empty() ? 0 : QRandomGenerator::global()->bounded(0,previousImages.size())};
    auto const randomNext{nextImages.empty() ? 0 : QRandomGenerator::global()->bounded(0,nextImages.size())};
    const bool previousIsEmpty{previousImages.empty()};
    const bool nextIsEmpty{nextImages.empty()};
    const bool playPrevious{QRandomGenerator::global()->bounded(0,1)%2 == 0};
    if(!previousIsEmpty && !nextIsEmpty)
    {
        readImage(playPrevious ? previousImages.at(randomPrevious) : nextImages.at(randomNext));
    }
    else if (previousIsEmpty)
    {
        readImage(nextImages.at(randomNext));
    }
    else if(nextIsEmpty)
    {
        readImage(previousImages.at(randomPrevious));
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
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        // Extract the local paths of the files.
        currentImageName = urlList[0].toString().right(urlList[0].toString().length() - 7);
        onOpen(currentImageName);
    }
}

// Handling all key events.
void ImageViewer::keyPressEvent(QKeyEvent *e)
{
    isRunningDiapo = false;
    switch (e->key())
    {
        case Qt::Key_Left:
            onPrevious();
        break;
        case Qt::Key_Right:
            onNext();
        break;
        case Qt::Key_F11:
            setFullScreen(!appIsFullScreen);
        break;
        default:
        break;
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent *ev)
{
    QString text{"x : "+QString::number(ev->pos().x())+" y : "+QString::number(ev->pos().y())};
    positionBar->showMessage(text);
}

// When the mouse is pressed, we leave the diapo
void ImageViewer::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
    isRunningDiapo = false;
}

void ImageViewer::setFullScreen(bool ok)
{
    appIsFullScreen = ok;
    (appIsFullScreen) ? showFullScreen() : showNormal();
}

void ImageViewer::showInfo()
{
    if(img.isNull()) return;
    ImageInfo *infoDialog = new ImageInfo(this,img,currentImageName);
    infoDialog->show();
}

ImageViewer::~ImageViewer()
{}
